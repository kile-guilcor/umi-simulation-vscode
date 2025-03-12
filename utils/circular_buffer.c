#include "circular_buffer.h"
#include <stdlib.h>
#include <string.h>

/**
 * Initialize a new circular buffer with given size
 * @param size Maximum number of elements in the buffer
 * @return Pointer to the newly created circular buffer
 */
circular_buffer_t* circular_buffer_init(size_t size) {
    circular_buffer_t* cb = (circular_buffer_t*)malloc(sizeof(circular_buffer_t));
    if (cb == NULL) {
        return NULL;
    }

    cb->buffer = (float*)malloc(size * sizeof(float));
    if (cb->buffer == NULL) {
        free(cb);
        return NULL;
    }

    cb->max = size;
    circular_buffer_reset(cb);
    return cb;
}

/**
 * Free all memory associated with the circular buffer
 * @param cb Pointer to the circular buffer
 */
void circular_buffer_free(circular_buffer_t* cb) {
    if (cb != NULL) {
        free(cb->buffer);
        free(cb);
    }
}

/**
 * Reset the circular buffer to empty state
 * @param cb Pointer to the circular buffer
 */
void circular_buffer_reset(circular_buffer_t* cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->full = 0;
}

/**
 * Push an item to the circular buffer
 * @param cb Pointer to the circular buffer
 * @param item Value to be pushed
 * @return 0 on success, -1 if buffer is full
 */
int circular_buffer_push(circular_buffer_t* cb, float item) {
    if (circular_buffer_full(cb)) {
        return -1;
    }

    cb->buffer[cb->head] = item;
    cb->head = (cb->head + 1) % cb->max;
    cb->full = (cb->head == cb->tail);
    return 0;
}

/**
 * Pop an item from the circular buffer
 * @param cb Pointer to the circular buffer
 * @param item Pointer to store the popped value
 * @return 0 on success, -1 if buffer is empty
 */
int circular_buffer_pop(circular_buffer_t* cb, float* item) {
    if (circular_buffer_empty(cb)) {
        return -1;
    }

    *item = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % cb->max;
    cb->full = 0;
    return 0;
}

/**
 * Check if the circular buffer is empty
 * @param cb Pointer to the circular buffer
 * @return 1 if empty, 0 otherwise
 */
int circular_buffer_empty(circular_buffer_t* cb) {
    return (!cb->full && (cb->head == cb->tail));
}

/**
 * Check if the circular buffer is full
 * @param cb Pointer to the circular buffer
 * @return 1 if full, 0 otherwise
 */
int circular_buffer_full(circular_buffer_t* cb) {
    return cb->full;
}

/**
 * Get the current number of elements in the circular buffer
 * @param cb Pointer to the circular buffer
 * @return Number of elements in the buffer
 */
size_t circular_buffer_size(circular_buffer_t* cb) {  // Changed return type to size_t
    if (cb->full) {
        return cb->max;
    } else if (cb->head >= cb->tail) {
        return cb->head - cb->tail;
    } else {
        return cb->max + cb->head - cb->tail;
    }
}

/**
 * Alias for circular_buffer_size
 * @param cb Pointer to the circular buffer
 * @return Number of elements in the buffer
 */
size_t circular_buffer_get_count(circular_buffer_t* cb) {
    return circular_buffer_size(cb);
}

/**
 * Get an item from the circular buffer at the specified index
 * @param cb Pointer to the circular buffer
 * @param index Index of the item to get (0 is oldest item)
 * @return Value at the specified index
 */
float circular_buffer_get(circular_buffer_t* cb, size_t index) {
    if (index >= circular_buffer_size(cb)) {
        return 0.0f;  // Return default value if index is out of bounds
    }

    size_t actual_index = (cb->tail + index) % cb->max;
    return cb->buffer[actual_index];
}
