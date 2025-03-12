#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stddef.h>

/**
 * Circular buffer structure
 */
typedef struct {
    float* buffer;    /* Buffer array */
    size_t head;      /* Index of the buffer head */
    size_t tail;      /* Index of the buffer tail */
    size_t max;       /* Maximum number of elements */
    int full;         /* Flag to indicate if buffer is full */
} circular_buffer_t;

circular_buffer_t* circular_buffer_init(size_t size);
void circular_buffer_free(circular_buffer_t* cb);
void circular_buffer_reset(circular_buffer_t* cb);
int circular_buffer_push(circular_buffer_t* cb, float item);
int circular_buffer_pop(circular_buffer_t* cb, float* item);
int circular_buffer_empty(circular_buffer_t* cb);
int circular_buffer_full(circular_buffer_t* cb);
size_t circular_buffer_size(circular_buffer_t* cb);
size_t circular_buffer_get_count(circular_buffer_t* cb);  // Add this line
float circular_buffer_get(circular_buffer_t* cb, size_t index);

#endif /* CIRCULAR_BUFFER_H */
