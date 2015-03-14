/**
 * @file       buf.h
 * @author     Emil Nilsson
 * @license    MIT
 * @date       2015
 */

#ifndef TAIL_BUF_H
#define TAIL_BUF_H

// for size_t
#include <stddef.h>
// for off_t
#include <sys/types.h>
// for assert
#include <assert.h>

/**
 * A text buffer.
 */
typedef struct tail_buf {
    /** The text data. */
    char * data;

    /** The length of the text data. */
    size_t len;

    /** The file offset of the start of the text data. */
    off_t start;
} tail_buf;

/**
 * Gets the offset for data in a text buffer.
 */
static inline off_t tail_buf_to_offset(tail_buf * buf, char * s)
{
    // include the index following the last index
    assert(s == NULL || s >= buf->data && s <= buf->data + buf->len);
    return buf->start + (s ? s - buf->data : 0);
}

/**
 * Gets data at an offset in a text buffer.
 */
static inline char * tail_buf_from_offset(tail_buf * buf, off_t off)
{
    // include the index following the last index
    assert(off >= buf->start && off <= buf->start + buf->len);
    return buf->data + (off - buf->start);
}

/**
 * Gets the length from the start of the data to an offset in a text buffer.
 */
static inline size_t tail_buf_to_len(tail_buf * buf, off_t off)
{
    // include the index following the last index
    assert(off >= buf->start && off <= buf->start + buf->len);
    return off - buf->start;
}

/**
 * Prepends a text buffer before another text buffer.
 */
extern int tail_prepend_buf(tail_buf * first, tail_buf * last);

/**
 * Frees the data allocated by a buffer.
 */
extern void tail_free_buf_data(tail_buf * buf);

/**
 * Copies a substring from a text buffer.
 */
extern char * tail_copy_from_buf_data(tail_buf * buf, char * start, char * end);

#endif

