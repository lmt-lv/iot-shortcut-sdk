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
- nRF Connect SDK 3.2.4
- nRF Connect SDK Toolchain 3.2.4

## Installation

### Automatic
1. Install LMT Shortcut SDK via VS Code plugin

### Manual
1. Download or clone this repository:
   ```bash
   git clone [repository-url]
   ```

### Add custom board definition path 
Add the IoT Shortcut SDK boards definition folder to the nRF Connect SDK configuration:
   - Follow the instructions at https://academy.nordicsemi.com/courses/nrf-connect-sdk-intermediate/lessons/lesson-3-adding-custom-board-support/topic/exercise-1/
   - **OR** copy the `lmtSDK/boards` folder to your sample project's or application `boards` folder

## Getting Started
1. Open a sample project folder in VS Code
2. Select the appropriate board target: `lmt9151som/nrf9151/ns`
3. Build and flash the project

## Samples
The SDK includes the following sample projects:
- **hello_c**: Simplest use case - minimal SDK initialization and basic functionality
- **hello2_c**: Basic SDK usage with additional debugging features
- **ek_demo**: Full-featured example with potentiometer, accelerometer (LIS3DH), and environmental sensor (BMP390) integration

**Important**: All projects using the LMT Shortcut SDK must include:
- The `sysbuild` subfolder (copy from root diretory directly to your project)
- The `sysbuild.conf` file for multi-image build configuration (copy from root diretory directly to your project)
- The `etc/COAP.json` configuration file (see samples for reference)

## Acknowledgments
Built on Nordic Semiconductor's nRF Connect SDK.

## License
This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for details.

Copyright 2025 Latvijas Mobilais Telefons