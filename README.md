# HDC2010 Environmental Sensor Project

A complete C application for reading and displaying data from the HDC2010 temperature and humidity sensor, with random data simulation capabilities.

## Features

- Real HDC2010 sensor integration (when hardware available)
- Realistic random data simulation
- Multiple output formats (Console, JSON, CSV)
- Configurable measurement intervals
- Environmental condition classification
- Graceful shutdown handling

# Clean previous build
make clean

# Build the project
make

# Run with default settings (simulation mode)
./build/hdc2010_app

# Run with specific options
./build/hdc2010_app -m json -i 1000 -c 5
