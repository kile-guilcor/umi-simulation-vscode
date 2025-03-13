#include <stdlib.h>
#include <time.h>

#include "app/app.h"
#include "app/ui/vars.h"
#include "app/ui/screens.h"
#include "app/sensor.h"

#define PLOT_POINT_COUNT 50

clock_t start;

extern objects_t objects;
extern Sensor sensor_arr[4];
extern int current_sensor;

void init_chart(lv_obj_t *chart) {
    // Configure chart properties
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    // lv_chart_set_range(objects.sensor_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_div_line_count(chart, 10, 10);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_point_count(chart, 50); // Display 50 points

    // Add the data series
    lv_chart_series_t *series = lv_chart_add_series(
        chart,
        lv_palette_main(LV_PALETTE_RED),
        LV_CHART_AXIS_PRIMARY_Y
    );
}

void update_chart(lv_obj_t *chart) {
    lv_chart_series_t *ser1 = lv_chart_get_series_next(chart, NULL);
    float min = sensor_arr[current_sensor].min;
    float max = sensor_arr[current_sensor].max;

    static lv_coord_t plot_data[PLOT_POINT_COUNT];
    static float sensor_data[PLOT_POINT_COUNT];

    // Get data points from current sensor
    int count = sensor_get_plot_data(&sensor_arr[current_sensor], sensor_data, PLOT_POINT_COUNT);

    // Convert float values to chart coordinates
    for (int i = 0; i < count; i++) {
        plot_data[i] = (lv_coord_t)sensor_data[i];
    }

    // Clear previous data and set new series
    lv_chart_set_ext_y_array(chart, ser1, plot_data);
    lv_chart_set_axis_range(chart, LV_CHART_AXIS_PRIMARY_Y, min, max);
    lv_scale_set_range(objects.y_scale_1, min, max);

    // Refresh the chart
    lv_chart_refresh(chart);
}

void app_setup(void) {
    // Set default values
    start = clock();
    sensors_init();
    set_var_date_info("2023-10-01");

    // Detects sensors, initializes, calibrates, etc.
    init_chart(objects.sensor_chart_1);
    init_chart(objects.sensor_chart_2);
}

void app_tick(void) {
    static int counter = 0;

    int minute_spent = (int)(((double)(clock() - start) / CLOCKS_PER_SEC) / 60);
    int memory_used = (int)(((float) circular_buffer_size(sensor_arr->data) / sensor_arr->data->max) * 100);

    set_var_runtime_m(minute_spent);
    set_var_memory(memory_used);
    update_chart(objects.sensor_chart_1);
    update_chart(objects.sensor_chart_2);

    counter++;

    if (counter % 250 != 0) {
        return;
    }

    // Reading sensor values
    for (int i = 0; i < 4; i++) {
        float sensor_value = (float)rand() / (float)RAND_MAX * 100.0f;

        sensor_add_reading(&sensor_arr[i], sensor_value);
        if (sensor_value > sensor_arr[i].max) {
            sensor_arr[i].max = sensor_value;
        }

        if (sensor_value < sensor_arr[i].min) {
            sensor_arr[i].min = sensor_value;
        }
    }

    // Reset Counter
    counter = 0;
}
