# IoT Shortcut SDK

The latest LMT IoT Shortcut SDK library API.

## Overview
IoT Shortcut SoM and SDK is designed to accelerate time-to-market by streamlining the data delivery process to platform. The SDK handles critical functionalities including connection management, FOTA updates, and logging, allowing developers to focus on their core application logic rather than infrastructure concerns.

## Requirements
### Hardware
- LMT IoT Shortcut nRF9151 SoM and carrier board
- JLink programmer/debugger (or compatible)
- Board power supply (e.g. nRF PPK2)
- USB-UART bridge for serial debug out (optional)

### Software
- VS Code
- nRF Connect SDK 2.6.0
- nRF Connect SDK Toolchain 2.6.0

## Installation

### Automatic
1. Install LMT Shortcut SDK via VS Code plugin

### Manual
1. Download or clone this repository:
   ```bash
   git clone [repository-url]
   ``` 

2. Add the IoT Shortcut SDK boards definition folder to the nRF Connect SDK configuration:
   - Follow the instructions at https://academy.nordicsemi.com/courses/nrf-connect-sdk-intermediate/lessons/lesson-3-adding-custom-board-support/topic/exercise-1/
   - **OR** copy the `lmtSDK/boards` folder to your sample project's or apllication `boards` folder

## Getting Started
1. Open a sample project folder in VS Code
2. Select the appropriate board target: `lmt_som_nrf9151_ns`
3. Build and flash the project

## Samples
The SDK includes the following sample projects:
- **hello_c**: Simplest use case - minimal SDK initialization and basic functionality
- **hello2_c**: Basic SDK usage with additional debugging features
- **ek_sensor_example**: Full-featured example with complete system logic (see sample README for details)

### Building Samples
1. Open the corresponding sample project folder in VS Code
2. Select the appropriate board configuration `lmt_som_nrf9151_ns`
3. Build the project
4. Flash to the device

**Important**: All projects using the LMT Shortcut SDK must include:
- The `child_image` subfolder (copy from SDK root folder to your project)
- The `etc/COAP.json` configuration file (see samples for reference)

## Acknowledgments
Built on Nordic Semiconductor's nRF Connect SDK.

## License
This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for details.

Copyright 2025 Latvijas Mobilais Telefons