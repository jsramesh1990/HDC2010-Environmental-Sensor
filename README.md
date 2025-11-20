# HDC2010 Environmental Sensor Project

A complete C application for reading and displaying data from the HDC2010 temperature and humidity sensor, with random data simulation capabilities.

## Features

- Real HDC2010 sensor integration (when hardware available)
- Realistic random data simulation
- Multiple output formats (Console, JSON, CSV)
- Configurable measurement intervals
- Environmental condition classification
- Graceful shutdown handling

## Building

```bash
make          # Build the application
make debug    # Build with debug symbols
make release  # Build with optimizations
make clean    # Clean build files
