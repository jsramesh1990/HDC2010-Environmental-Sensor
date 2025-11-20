#ifndef RANDOM_SIMULATOR_H
#define RANDOM_SIMULATOR_H

#include "hdc2010.h"

void random_simulator_init(void);
bool random_simulator_read(hdc2010_data_t *data);
void random_simulator_set_pattern(int pattern_type);
const char* random_simulator_get_environment_description(float temp, float humidity);

#endif
