/**
 * @file       tail.c
 * @author     Emil Nilsson
 * @license    MIT
 * @date       2015
 */

// for tail_buf_to_offset, tail_buf_from_offset, tail_buf_to_len, tail_prepend_buf, tail_free_buf_data,
// tail_copy_from_buf_data, tail_buf
#include "buf.h"
// for read, lseek, SEEK_CUR, SEEK_SET, SEEK_END
#include <unistd.h>
// for malloc
#include <stdlib.h>
// for size_t, ssize_t
#include <stddef.h>
// for off_t
#include <sys/types.h>
// for memrchr
#include <string.h>
// for assert
#include <assert.h>

/**
 * A context used when searching a file buffer for lines.
 */
typedef struct tail_search_ctx {
    /** The offset at the end of the file. */
    off_t file_end_off;

    /** The offset the search should continue from. */
    off_t last_off;

    /** The offset of the start of the current line. */
    off_t start_off;

    /** The offset of the end of the current line. */
    off_t end_off;
} tail_search_ctx;

/**
 * Reads data from the file.
 */
static ssize_t tail_read_block(int fd, tail_buf * buf, size_t buf_size)
{
    // bytes left to read
    size_t remaining = buf_size;
    // number of bytes read
    size_t bytes_read;
    // the write pointer
    char * write_next;
    // the start offset
    off_t start;

    // get the start position
    if ((start = lseek(fd, 0, SEEK_CUR)) == (off_t)-1) {
        return -1;
    }

    // allocate the buffer
    if ((buf->data = malloc(buf_size)) == NULL) {
        return -1;
    }

    write_next = buf->data;
    buf->len = buf_size;
    buf->start = start;

    // read until the entire buffer is filled
    while (remaining > 0 && (bytes_read = read(fd, write_next, remaining)) > 0) {
        remaining -= bytes_read;
        write_next += bytes_read;
    }

    // fail if the entire buffer wasn't read
    return bytes_read >= 0 && remaining == 0 ? 0 : -1;
}

/**
 * Moves to the end of the file.
 */
static off_t tail_seek_end(int fd, off_t * off_p)
{
    off_t new_off = lseek(fd, 0, SEEK_END);
    if (new_off != (off_t)-1 && off_p) {
        *off_p = new_off;
    }
    return new_off;
}

/**
 * Moves back to the previous block in the file.
 */
static off_t tail_seek_prev(int fd, off_t * off_p, size_t * buf_size_p)
{
    // the new start offset
    off_t new_off = *off_p - *buf_size_p;
    new_off = lseek(fd, new_off >= 0 ? new_off : 0, SEEK_SET);
    if (new_off != (off_t)-1) {
        // the buffer size for the block
        *buf_size_p = *off_p - new_off;
        *off_p = new_off;
    }

    return new_off;
}

/**
 * Finds the previous occurance of a character in a text buffer.
 */
static off_t tail_find_prev_in_buf(tail_buf * buf, char c, off_t end_off)
{
    char * end = tail_buf_from_offset(buf, end_off);
    char * found;
    if ((found = memrchr(buf->data, c, tail_buf_to_len(buf, end_off))) == NULL) {;
        // not found
        return (off_t)-1;
    }

    return tail_buf_to_offset(buf, found);
}

/**
 * Copies a line defined in a search context from a text buffer.
 */
static char * tail_copy_line(tail_search_ctx * ctx, tail_buf * buf)
{
    char * start = tail_buf_from_offset(buf, ctx->start_off);
    char * end = tail_buf_from_offset(buf, ctx->end_off);
    return tail_copy_from_buf_data(buf, start, end);
}

/**
 * Initializes a search context.
 */
static int tail_init_search_ctx(tail_search_ctx * ctx, int fd)
{
    off_t end_off;

    // find the offset of the end of the file
    if ((end_off = tail_seek_end(fd, NULL)) == (off_t)-1) {
        return -1;
    }

    ctx->file_end_off = end_off;
    ctx->last_off = end_off;
    ctx->end_off = end_off;

    return 0;
}

/**
 * Finds and copies the previous line from a text buffer.
 */
static char * tail_get_previous_line(tail_search_ctx * ctx, tail_buf * buf)
{
    // the offset of the previous newline
    off_t newline_off;
    // the copied line
    char * line = NULL;

    assert(ctx->last_off > 0);

    // find the previous newline
    if ((newline_off = tail_find_prev_in_buf(buf, '\n', ctx->last_off)) == (off_t)-1) {
        if (buf->start == 0) {
            // copy from the start of the file
            ctx->start_off = 0;
            line = tail_copy_line(ctx, buf);

            // reached the start of the file
            ctx->last_off = 0;
        }
    } else {
        if (newline_off == ctx->file_end_off - 1) {
            // ignore empty last line
            ctx->last_off = newline_off;
            line = tail_get_previous_line(ctx, buf);
        } else {
            // copy the line
            ctx->start_off = newline_off+1;
            line = tail_copy_line(ctx, buf);

            // include the newline for the previous line
            ctx->end_off = newline_off + 1;
            ctx->last_off = newline_off;
        }
    }

    return line;
}

int tail_read_lines(int fd, size_t buf_size, char ** lines, int num_lines)
{
    // the search context
    tail_search_ctx ctx;
    // buffer for data read from fle
    tail_buf read_buf = { .data = NULL, .len = 0 , .start = 0 };
    // buffer for data to search in
    tail_buf work_buf = { .data = NULL, .len = 0 , .start = 0 };
    // the number of lines left to read
    int lines_left = num_lines;
    // the offset to read from
    off_t read_off;

    assert(fd > 0);

    // initialize search context for searching from the end of the file
    if (tail_init_search_ctx(&ctx, fd) != 0) {
        goto fail;
    }

    // start from the end
    read_off = ctx.file_end_off;

    // read until the entire file is read or all lines have been read
    while (lines_left > 0 && read_off > 0) {
        // size of buffer which will be read
        size_t actual_buf_size = buf_size;
        // the previous line
        char * line = NULL;

        // update the offset and read the block
        if (tail_seek_prev(fd, &read_off, &actual_buf_size) == (off_t)-1) {
            goto fail;
        }
        if (tail_read_block(fd, &read_buf, actual_buf_size) < 0) {
            goto fail;
        }

        // concat the new data to the work buffer
        if (tail_prepend_buf(&read_buf, &work_buf) == -1) {
            goto fail;
        }
        tail_free_buf_data(&read_buf);

        // get lines in work buffer
        while (lines_left > 0 && ctx.last_off > 0 && (line = tail_get_previous_line(&ctx, &work_buf)) != NULL) {
            lines[lines_left - 1] = line;
            lines_left--;
        }
    }

    tail_free_buf_data(&work_buf);

    // return the actual number of lines read
    return num_lines - lines_left;

fail:
    // TODO: free lines

    tail_free_buf_data(&work_buf);
    tail_free_buf_data(&read_buf);

    return -1;
}

