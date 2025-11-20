#include <stdio.h>
#include <time.h>
#include <string.h>
#include "display.h"
#include "random_simulator.h"  // Include this to get the function declaration

static display_mode_t current_display_mode = DISPLAY_CONSOLE;
static int csv_header_printed = 0;

void display_init(display_mode_t mode) {
    current_display_mode = mode;
    csv_header_printed = 0;
    
    if (mode == DISPLAY_CSV && !csv_header_printed) {
        printf("timestamp,measurement_id,temperature_c,temperature_f,humidity,environment\n");
        csv_header_printed = 1;
    }
}

void display_sensor_data(const hdc2010_data_t *data, int measurement_id) {
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    float temp_f = data->temperature * 9.0f / 5.0f + 32.0f;
    const char* env_desc = random_simulator_get_environment_description(
        data->temperature, data->humidity);
    
    switch (current_display_mode) {
        case DISPLAY_CONSOLE:
            printf("[%s] Measurement #%d\n", timestamp, measurement_id);
            printf("  Temperature: %6.2f °C (%6.2f °F)\n", data->temperature, temp_f);
            printf("  Humidity:    %6.2f%% RH\n", data->humidity);
            printf("  Environment: %s\n", env_desc);
            printf("  Status:      %s\n\n", data->data_ready ? "Data Ready" : "No Data");
            break;
            
        case DISPLAY_JSON:
            printf("{\n");
            printf("  \"timestamp\": \"%s\",\n", timestamp);
            printf("  \"measurement_id\": %d,\n", measurement_id);
            printf("  \"temperature_c\": %.2f,\n", data->temperature);
            printf("  \"temperature_f\": %.2f,\n", temp_f);
            printf("  \"humidity\": %.2f,\n", data->humidity);
            printf("  \"environment\": \"%s\",\n", env_desc);
            printf("  \"data_ready\": %s\n", data->data_ready ? "true" : "false");
            printf("}\n");
            break;
            
        case DISPLAY_CSV:
            printf("%s,%d,%.2f,%.2f,%.2f,%s\n", 
                   timestamp, measurement_id, data->temperature, 
                   temp_f, data->humidity, env_desc);
            break;
    }
    
    fflush(stdout); // Ensure immediate output
}

void display_cleanup(void) {
    if (current_display_mode == DISPLAY_CONSOLE) {
        printf("=== Display system shutdown ===\n");
    }
}
