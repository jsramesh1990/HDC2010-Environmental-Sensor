#ifndef HDC2010_H
#define HDC2010_H

#include <stdint.h>
#include <stdbool.h>

// I2C Addresses
#define HDC2010_ADDR_GROUND 0x40
#define HDC2010_ADDR_VDD    0x41

// Register Addresses
#define HDC2010_REG_TEMP_LOW         0x00
#define HDC2010_REG_TEMP_HIGH        0x01
#define HDC2010_REG_HUMIDITY_LOW     0x02
#define HDC2010_REG_HUMIDITY_HIGH    0x03
#define HDC2010_REG_INTERRUPT_DRDY   0x04
#define HDC2010_REG_TEMP_MAX         0x05
#define HDC2010_REG_HUMIDITY_MAX     0x06
#define HDC2010_REG_INTERRUPT_ENABLE 0x07
#define HDC2010_REG_TEMP_OFFSET      0x08
#define HDC2010_REG_HUMIDITY_OFFSET  0x09
#define HDC2010_REG_TEMP_THRES_L     0x0A
#define HDC2010_REG_TEMP_THRES_H     0x0B
#define HDC2010_REG_HUMIDITY_THRES_L 0x0C
#define HDC2010_REG_HUMIDITY_THRES_H 0x0D
#define HDC2010_REG_CONFIG           0x0E
#define HDC2010_REG_MEAS_CONFIG      0x0F
#define HDC2010_REG_MANUFACTURER_LOW 0xFC
#define HDC2010_REG_MANUFACTURER_HIGH 0xFD
#define HDC2010_REG_DEVICE_ID_LOW    0xFE
#define HDC2010_REG_DEVICE_ID_HIGH   0xFF

// Resolution settings
typedef enum {
    HDC2010_RES_14BIT = 0,
    HDC2010_RES_11BIT = 1,
    HDC2010_RES_9BIT  = 2
} hdc2010_resolution_t;

// Measurement configuration
typedef enum {
    HDC2010_MEAS_BOTH = 0,    // Humidity + Temperature
    HDC2010_MEAS_TEMP_ONLY = 1 // Temperature only
} hdc2010_meas_config_t;

// Auto measurement modes
typedef enum {
    HDC2010_AMM_DISABLED = 0,
    HDC2010_AMM_1_120HZ = 1,   // 1 sample every 2 minutes
    HDC2010_AMM_1_60HZ  = 2,   // 1 sample every minute
    HDC2010_AMM_0_1HZ   = 3,   // 1 sample every 10 seconds
    HDC2010_AMM_0_2HZ   = 4,   // 1 sample every 5 seconds
    HDC2010_AMM_1HZ     = 5,   // 1 sample per second
    HDC2010_AMM_2HZ     = 6,   // 2 samples per second
    HDC2010_AMM_5HZ     = 7    // 5 samples per second
} hdc2010_amm_t;

// Sensor data structure
typedef struct {
    float temperature;
    float humidity;
    bool data_ready;
    uint32_t timestamp;
} hdc2010_data_t;

// Function prototypes
bool hdc2010_init(uint8_t i2c_addr);
bool hdc2010_read_temperature_humidity(hdc2010_data_t *data);
bool hdc2010_configure_measurement(hdc2010_resolution_t temp_res, 
                                  hdc2010_resolution_t hum_res,
                                  hdc2010_meas_config_t meas_config);
bool hdc2010_set_auto_measurement(hdc2010_amm_t amm_mode);
bool hdc2010_start_measurement(void);
bool hdc2010_is_data_ready(void);
bool hdc2010_set_heater(bool enable);
bool hdc2010_soft_reset(void);
uint16_t hdc2010_read_manufacturer_id(void);
uint16_t hdc2010_read_device_id(void);

#endif // HDC2010_H
