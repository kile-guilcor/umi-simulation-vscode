#include "circular_buffer.h"
#include <stdlib.h>
#include <string.h>

circular_buffer_t* circular_buffer_init(size_t size) {
    circular_buffer_t *cb = (circular_buffer_t *)malloc(sizeof(circular_buffer_t));
    cb->buffer = (float *)malloc(size * sizeof(float));
    cb->max = size;
    circular_buffer_reset(cb);
    return cb;
}

void circular_buffer_free(circular_buffer_t *cb) {
    free(cb->buffer);
    free(cb);
}

void circular_buffer_reset(circular_buffer_t *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->full = 0;
}

int circular_buffer_push(circular_buffer_t *cb, float item) {
    if (circular_buffer_full(cb)) {
        return -1;
    }
    cb->buffer[cb->head] = item;
    cb->head = (cb->head + 1) % cb->max;
    cb->full = (cb->head == cb->tail);
    return 0;
}

int circular_buffer_pop(circular_buffer_t *cb, float *item) {
    if (circular_buffer_empty(cb)) {
        return -1;
    }
    *item = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % cb->max;
    cb->full = 0;
    return 0;
}

int circular_buffer_empty(circular_buffer_t *cb) {
    return (!cb->full && (cb->head == cb->tail));
}

int circular_buffer_full(circular_buffer_t *cb) {
    return cb->full;
}

size_t circular_buffer_size(circular_buffer_t *cb) {
    size_t size = cb->max;
    if (!cb->full) {
        if (cb->head >= cb->tail) {
            size = cb->head - cb->tail;
        } else {
            size = cb->max + cb->head - cb->tail;
        }
    }
    return size;
}
