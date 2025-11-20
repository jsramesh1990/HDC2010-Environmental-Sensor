#ifndef DISPLAY_H
#define DISPLAY_H

#include "hdc2010.h"

// Forward declaration to avoid circular dependency
const char* random_simulator_get_environment_description(float temp, float humidity);

typedef enum {
    DISPLAY_CONSOLE,
    DISPLAY_JSON,
    DISPLAY_CSV
} display_mode_t;

void display_init(display_mode_t mode);
void display_sensor_data(const hdc2010_data_t *data, int measurement_id);
void display_cleanup(void);

#endif
