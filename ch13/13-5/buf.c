/**
 * @file       buf.c
 * @author     Emil Nilsson
 * @license    MIT
 * @date       2015
 */

#include "buf.h"

// for malloc, free
#include <stdlib.h>
// for strndup, memcpy, memrchr
#include <string.h>
// for assert
#include <assert.h>

int tail_prepend_buf(tail_buf * first, tail_buf * last)
{
    // the concatenated text buffer data
    char * new_data = NULL;

    if (last->data) {
        assert(first->start + first->len == last->start);

        if ((new_data = malloc(first->len + last->len)) == NULL) {
            return -1;
        }
        // concatenate the buffer data
        memcpy(new_data, first->data, first->len);
        memcpy(new_data + first->len, last->data, last->len);
        free(last->data);
    } else {
        if ((new_data = strndup(first->data, first->len)) == NULL) {
            return -1;
        }
    }

    // update the buffer
    last->data = new_data;
    last->len = first->len + last->len;
    last->start = first->start;

    return 0;
}

void tail_free_buf_data(tail_buf * buf)
{
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
    }
}

char * tail_copy_from_buf_data(tail_buf * buf, char * start, char * end)
{
    // the copied data
    char * data;

    assert(end >= start && start >= buf->data && end <= buf->data + buf->len);

    if ((data = strndup(start, end - start)) == NULL) {
        return NULL;
    }

    return data;
}

