#include <stdlib.h>

#include "app/app.h"
#include "app/ui/vars.h"
#include "app/ui/screens.h"

extern objects_t objects;

void init_chart() {
    // lv_obj_set_size(objects.sensor_chart, 200, 150);
    // lv_obj_align(objects.sensor_chart, LV_ALIGN_CENTER, 0, 0);

    // Configure chart properties
    lv_chart_set_type(objects.sensor_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(objects.sensor_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_div_line_count(objects.sensor_chart, 5, 5);
    lv_chart_set_point_count(objects.sensor_chart, 50); // Display 50 points

    // Add the data series
    lv_chart_series_t *series = lv_chart_add_series(
        objects.sensor_chart,
        lv_palette_main(LV_PALETTE_RED),
        LV_CHART_AXIS_PRIMARY_Y
    );
}

void app_setup(void) {
    // Set default values
    set_var_sensor_value(0.0f);
    set_var_sensor_min(0.0f);
    set_var_sensor_max(0.0f);
    set_var_sensor_accuracy(0.01f);
    set_var_sensor_unit_info("°C");
    set_var_date_info("2023-10-01");
    set_var_sensor_name_info("PT100");

    // Detects sensors, initializes, calibrates, etc.

    init_chart();
}

void app_tick(void) {
    static int counter = 0;

    // Reading sensor values
    float sensor_value = (float)rand() / (float)RAND_MAX * 100.0f;

    // Update sensor value
    if (counter % 500 == 0) {
        set_var_sensor_value(sensor_value);
    }

    // Add sensor value to chart
    if (counter % 500 == 0 && objects.sensor_chart) {
        lv_chart_series_t *ser1 = lv_chart_get_series_next(objects.sensor_chart, NULL);
        if (ser1) {
            lv_chart_set_next_value(objects.sensor_chart, ser1, sensor_value);
            lv_chart_refresh(objects.sensor_chart); // Refresh the chart to show the new data
        }
    }

    counter++;
}
