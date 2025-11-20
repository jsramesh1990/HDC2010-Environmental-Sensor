#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "hdc2010.h"
#include "display.h"
#include "random_simulator.h"

volatile sig_atomic_t keep_running = 1;

void signal_handler(int signal) {
    keep_running = 0;
    printf("\nShutting down HDC2010 application...\n");
}

void print_usage(const char* program_name) {
    printf("HDC2010 Sensor Application\n");
    printf("Usage: %s [options]\n", program_name);
    printf("Options:\n");
    printf("  -h, --help          Show this help message\n");
    printf("  -i, --interval MS   Set measurement interval in milliseconds (default: 2000)\n");
    printf("  -m, --mode MODE     Set display mode: console, json, csv (default: console)\n");
    printf("  -c, --count NUM     Number of measurements to take (default: infinite)\n");
    printf("  -r, --real          Use real HDC2010 sensor (default: simulation)\n");
}

int main(int argc, char *argv[]) {
    // Default configuration
    int measurement_interval = 2000; // 2 seconds
    display_mode_t display_mode = DISPLAY_CONSOLE;
    int max_measurements = 0; // 0 = infinite
    int use_simulation = 1;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if ((strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interval") == 0) && i + 1 < argc) {
            measurement_interval = atoi(argv[++i]);
            if (measurement_interval < 100) {
                printf("Warning: Interval too short, setting to minimum 100ms\n");
                measurement_interval = 100;
            }
        } else if ((strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--mode") == 0) && i + 1 < argc) {
            if (strcmp(argv[++i], "json") == 0) display_mode = DISPLAY_JSON;
            else if (strcmp(argv[i], "csv") == 0) display_mode = DISPLAY_CSV;
            else display_mode = DISPLAY_CONSOLE;
        } else if ((strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--count") == 0) && i + 1 < argc) {
            max_measurements = atoi(argv[++i]);
            if (max_measurements < 1) {
                printf("Error: Count must be at least 1\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--real") == 0) {
            use_simulation = 0;
        }
    }
    
    // Setup signal handling for graceful shutdown
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    printf("=== HDC2010 Environmental Sensor Application ===\n");
    printf("Configuration:\n");
    printf("  Interval: %d ms\n", measurement_interval);
    printf("  Display Mode: ");
    switch(display_mode) {
        case DISPLAY_CONSOLE: printf("Console\n"); break;
        case DISPLAY_JSON: printf("JSON\n"); break;
        case DISPLAY_CSV: printf("CSV\n"); break;
    }
    printf("  Max Measurements: %s\n", max_measurements ? "Limited" : "Infinite");
    printf("  Mode: %s\n", use_simulation ? "Simulation" : "Real Sensor");
    printf("Press Ctrl+C to exit\n\n");
    
    // Initialize display system
    display_init(display_mode);
    
    // Initialize sensor or simulator
    hdc2010_data_t sensor_data;
    if (use_simulation) {
        random_simulator_init();
    } else {
        if (!hdc2010_init(HDC2010_ADDR_GROUND)) {
            printf("Failed to initialize HDC2010 sensor!\n");
            return 1;
        }
        // Configure the sensor for real operation
        hdc2010_configure_measurement(HDC2010_RES_14BIT, HDC2010_RES_14BIT, HDC2010_MEAS_BOTH);
        hdc2010_set_auto_measurement(HDC2010_AMM_1HZ);
    }
    
    // Main measurement loop
    int measurement_count = 0;
    while (keep_running) {
        // Read sensor data
        bool success;
        if (use_simulation) {
            success = random_simulator_read(&sensor_data);
        } else {
            success = hdc2010_read_temperature_humidity(&sensor_data);
        }
        
        if (success) {
            // Display the data
            display_sensor_data(&sensor_data, measurement_count + 1);
            
            measurement_count++;
            
            // Check if we've reached the maximum measurements
            if (max_measurements > 0 && measurement_count >= max_measurements) {
                printf("\nReached maximum measurement count (%d)\n", max_measurements);
                break;
            }
        } else {
            printf("Error reading sensor data!\n");
        }
        
        // Wait for next measurement
        usleep(measurement_interval * 1000);
    }
    
    // Cleanup
    display_cleanup();
    printf("Application terminated. Total measurements: %d\n", measurement_count);
    
    return 0;
}
