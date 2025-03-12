#ifndef SENSOR_H
#define SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils/circular_buffer.h"

#define BUFFER_SIZE 100

// Typedefs
typedef struct {
    circular_buffer_t *data;  // Historical sensor values
    float min;                // Minimum possible reading
    float max;                // Maximum possible reading
    float accuracy;           // Sensor accuracy/precision
    char unit[32];            // Measurement unit (e.g., "Celsius")
    char name[64];            // Sensor name/identifier
    char location[64];        // Where the sensor is placed
    char last_calibration[20]; // Date of last calibration
} Sensor;

void sensor_init(Sensor* sensor, const char* name, const char* unit, float min, float max, float accuracy);
void sensors_init();
void sensor_add_reading(Sensor* sensor, float value);
float sensor_get_reading(Sensor* sensor);
int sensor_get_plot_data(Sensor* sensor, float* data, int size);

#ifdef __cplusplus
}
#endif

#endif // SENSOR_H
