#include <stdlib.h>
#include <string.h>
#include "app/sensor.h"
#include "utils/circular_buffer.h"

Sensor sensor_arr[4];

void sensor_init(Sensor* sensor, const char* name, const char* unit, float min, float max, float accuracy) {
    if (!sensor || !name || !unit) {
        return;
    }

    // Create circular buffer to store sensor readings
    sensor->data = circular_buffer_init(BUFFER_SIZE);
    if (!sensor->data) {
        return;
    }

    // Store sensor parameters
    sensor->min = min;
    sensor->max = max;
    sensor->accuracy = accuracy;

    strncpy(sensor->unit, unit, sizeof(sensor->unit) - 1);
    sensor->unit[sizeof(sensor->unit) - 1] = '\0';

    strncpy(sensor->name, name, sizeof(sensor->name) - 1);
    sensor->name[sizeof(sensor->name) - 1] = '\0';
}

void sensors_init() {
    sensor_init(&sensor_arr[0], "Temperature", "°C", 999.9f, -999.9f, 0.5f);
    sensor_init(&sensor_arr[1], "Humidity", "%", 999.9f, -999.9f, 2.0f);
    sensor_init(&sensor_arr[2], "Pressure", "hPa", 999.9f, -999.9f, 1.0f);
    sensor_init(&sensor_arr[3], "Light", "lux", 999.9f, -999.9f, 10.0f);
}

void sensor_add_reading(Sensor* sensor, float value) {
    if (!sensor || !sensor->data) {
        return;
    }
    circular_buffer_push(sensor->data, value);
}

float sensor_get_reading(Sensor* sensor) {
    if (!sensor || !sensor->data) {
        return 0.0f;
    }
    size_t size = circular_buffer_size(sensor->data);
    if (size == 0) {
        return 0.0f;  // Return default value for empty buffer
    }
    return circular_buffer_get(sensor->data, size - 1);  // Get the newest item
}

int sensor_get_plot_data(Sensor* sensor, float* data, int size) {
    if (!sensor || !sensor->data || !data || size <= 0) {
        return 0;
    }

    size_t count = circular_buffer_size(sensor->data);

    int copy_size = (count < (size_t)size) ? (int)count : size;

    for (int i = 0; i < copy_size; i++) {
        data[i] = circular_buffer_get(sensor->data, count - 1 - i);
    }

    for (int i = copy_size; i < size; i++) {
        data[i] = 0.0f;
    }

    return copy_size;
}
