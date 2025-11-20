#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "random_simulator.h"

static int simulator_initialized = 0;
static unsigned long measurement_count = 0;

// Realistic environmental patterns
typedef struct {
    float base_temp;
    float base_humidity;
    float temp_variation;
    float hum_variation;
    int pattern_type;
} environment_pattern_t;

// Different environmental scenarios
static environment_pattern_t patterns[] = {
    {25.0f, 45.0f, 5.0f, 15.0f, 0},  // Normal indoor
    {20.0f, 60.0f, 3.0f, 10.0f, 1},  // Comfortable
    {30.0f, 30.0f, 8.0f, 20.0f, 2},  // Hot and dry
    {15.0f, 80.0f, 2.0f, 5.0f, 3},   // Cool and humid
    {22.0f, 50.0f, 4.0f, 12.0f, 4}   // Ideal
};

void random_simulator_init(void) {
    if (!simulator_initialized) {
        srand(time(NULL));
        simulator_initialized = 1;
    }
}

bool random_simulator_read(hdc2010_data_t *data) {
    if (!simulator_initialized) {
        return false;
    }
    
    // Select a random environmental pattern
    int pattern_idx = rand() % (sizeof(patterns) / sizeof(patterns[0]));
    environment_pattern_t *pattern = &patterns[pattern_idx];
    
    // Add some realistic variations
    float time_variation = sin(measurement_count * 0.1f) * 2.0f;
    float random_temp_noise = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
    float random_hum_noise = ((float)rand() / RAND_MAX - 0.5f) * 5.0f;
    
    // Generate temperature with realistic patterns
    data->temperature = pattern->base_temp + 
                       time_variation + 
                       ((float)rand() / RAND_MAX - 0.5f) * pattern->temp_variation +
                       random_temp_noise;
    
    // Generate humidity (often inversely related to temperature)
    data->humidity = pattern->base_humidity - 
                    time_variation * 2.0f +  // Humidity often decreases when temp increases
                    ((float)rand() / RAND_MAX - 0.5f) * pattern->hum_variation +
                    random_hum_noise;
    
    // Ensure realistic bounds
    if (data->temperature < -40.0f) data->temperature = -40.0f;
    if (data->temperature > 125.0f) data->temperature = 125.0f;
    if (data->humidity < 0.0f) data->humidity = 0.0f;
    if (data->humidity > 100.0f) data->humidity = 100.0f;
    
    data->data_ready = true;
    data->timestamp = measurement_count;
    
    measurement_count++;
    
    return true;
}

void random_simulator_set_pattern(int pattern_type) {
    // Could be extended to allow specific pattern selection
}

const char* random_simulator_get_environment_description(float temp, float humidity) {
    if (temp < 10.0f) {
        if (humidity > 80.0f) return "Cold and Humid";
        else return "Cold and Dry";
    } else if (temp < 20.0f) {
        if (humidity > 70.0f) return "Cool and Humid";
        else if (humidity < 30.0f) return "Cool and Dry";
        else return "Comfortable Cool";
    } else if (temp < 27.0f) {
        if (humidity > 60.0f) return "Warm and Humid";
        else if (humidity < 30.0f) return "Warm and Dry";
        else return "Ideal Comfort";
    } else {
        if (humidity > 50.0f) return "Hot and Humid";
        else return "Hot and Dry";
    }
}
