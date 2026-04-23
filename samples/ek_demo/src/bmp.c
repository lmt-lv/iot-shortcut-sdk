/*
 * BMP390 Sensor Driver (Bosch Sensortec)
 *
 * This file provides functions to initialize and read data from the BMP390 pressure and temperature
 * sensor over I2C.
 *
 * The BMP390 is a digital barometric pressure and temperature sensor. It is used to measure
 * atmospheric pressure (in Pascals) and temperature (in degrees Celsius). The sensor requires
 * calibration and compensation to convert raw readings into accurate values.
 *
 * All pressure values returned are in Pascals (Pa), and temperature values are in hundredths of
 * degrees Celsius (°C × 100).
 *
 * Functions in this file:
 *  - Initialize the sensor
 *  - Read and compensate raw sensor data
 *  - Return accurate pressure and temperature values
 *
 * Key steps and calculations are explained in comments.
 */

#include "bmp.h"
#include "lmt_storage_manager.h" // For log functions
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/byteorder.h>

// BMP390 datasheet:
// https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp390-ds002.pdf
// Register addresses used to communicate with the BMP390 sensor
#define BMP390_REG_CHIP_ID  0x00 // Register to read the sensor's chip ID (should be 0x60)
#define BMP390_REG_STATUS   0x03 // Status register
#define BMP390_REG_DATA_0   0x04 // Start of pressure/temperature data registers
#define BMP390_REG_PWR_CTRL 0x1B // Power control register
#define BMP390_REG_OSR      0x1C // Oversampling settings register
#define BMP390_REG_ODR      0x1D // Output data rate register
#define BMP388_REG_CALIB0   0x31 // Start of calibration data registers
#define BMP390_REG_CMD      0x7E // Command register (e.g., for soft reset)

// BMP390 commands and configuration values
#define BMP390_CMD_SOFT_RESET  0xB6 // Command to reset the sensor
#define BMP390_CHIP_ID         0x60 // Expected chip ID value for BMP390
#define BMP390_PWR_CTRL_ENABLE 0x33 // Enable pressure, temperature, and normal mode
#define BMP390_OSR_DEFAULT     0x1B // 8x oversampling for both pressure and temperature
#define BMP390_ODR_DEFAULT     0x07 // Output data rate: 640 ms sampling period

// Structure to hold calibration data read from the sensor.
// These values are used to convert raw sensor readings into accurate pressure and temperature.
struct bmp390_cal_data
{
    uint16_t t1;
    uint16_t t2;
    int8_t t3;
    int16_t p1;
    int16_t p2;
    int8_t p3;
    int8_t p4;
    uint16_t p5;
    uint16_t p6;
    int8_t p7;
    int8_t p8;
    int16_t p9;
    int8_t p10;
    int8_t p11;
} __packed;

// I2C device pointer for BMP390 sensor (configured via device tree)
static const struct i2c_dt_spec bmp_i2c = I2C_DT_SPEC_GET(DT_NODELABEL(bmp390));
// Calibration data storage
static struct bmp390_cal_data calib_data;
// Variable to store compensated temperature (needed for pressure calculation)
static int64_t compensated_t_for_p;

/**
 * @brief Wake up BMP390 sensor by sending a dummy byte over I2C.
 *
 * This function ensures the sensor is ready to communicate. It tries up to 10 times.
 *
 * @return true if the sensor responded, false otherwise
 */
bool wakeupBmp()
{
    int error      = 0;
    int attemps    = 10;
    uint8_t byte_0 = 0x77; // Dummy value to wake up the sensor

    if(!device_is_ready(bmp_i2c.bus))
    {
        logWarning("i2c bus not ready");
        return false;
    }

    // Try to write until the sensor responds or attempts run out
    do
    {
        error = i2c_write_dt(&bmp_i2c, &byte_0, 1);
    } while((error < 0) && (--attemps > 0));

    if(error == 4)
    {
        logWarning("BMP waikeup failed");
        return false;
    }

    return (attemps > 0);
}

/**
 * @brief Read calibration data from BMP390 sensor.
 *
 * The sensor stores factory calibration values in its memory. These are needed to convert raw
 * readings into accurate pressure and temperature values.
 *
 * @return 0 on success, negative error code on failure
 */
int readCalibData(void)
{
    int error            = 0;
    uint8_t read_address = BMP388_REG_CALIB0;

    // Read calibration data from 0x31 to 0x45 (20 bytes)
    error = i2c_write_read_dt(&bmp_i2c, &read_address, sizeof(read_address), &calib_data,
                              sizeof(calib_data));
    if(error < 0)
    {
        logWarning("BMP390: Failed to read calibration data");
        return error;
    }

    // Convert from little-endian to CPU endianness if necessary
    calib_data.t1 = sys_le16_to_cpu(calib_data.t1);
    calib_data.t2 = sys_le16_to_cpu(calib_data.t2);
    calib_data.p1 = sys_le16_to_cpu(calib_data.p1);
    calib_data.p2 = sys_le16_to_cpu(calib_data.p2);
    calib_data.p5 = sys_le16_to_cpu(calib_data.p5);
    calib_data.p6 = sys_le16_to_cpu(calib_data.p6);
    calib_data.p9 = sys_le16_to_cpu(calib_data.p9);

    return 0;
}

/**
 * @brief Convert raw temperature value to real temperature using calibration data.
 *
 * The sensor provides a raw temperature value. This function uses calibration data to
 * "compensate" (correct) the value, so it matches the real temperature.
 *
 * @param raw_temp Raw temperature value from sensor
 * @return Compensated temperature in hundredths of degrees Celsius (°C × 100)
 */
int32_t bmp390CompensateTemp(uint32_t raw_temp)
{
    // The following calculations are based on the official Bosch BMP3-Sensor-API.
    // Adapted from: https://github.com/BoschSensortec/BMP3-Sensor-API/blob/master/bmp3.c
    int64_t partial_data1;
    int64_t partial_data2;
    int64_t partial_data3;
    int64_t partial_data4;
    int64_t partial_data5;

    // Step-by-step calculation for temperature compensation
    partial_data1 = ((int64_t)raw_temp - (256 * calib_data.t1));
    partial_data2 = calib_data.t2 * partial_data1;
    partial_data3 = (partial_data1 * partial_data1);
    partial_data4 = (int64_t)partial_data3 * calib_data.t3;
    partial_data5 = ((int64_t)(partial_data2 * 262144) + partial_data4);

    // Store intermediate result for use in pressure compensation
    compensated_t_for_p = partial_data5 / 4294967296;

    // Final temperature value in hundredths of a degree Celsius
    int64_t tmp = ((compensated_t_for_p * 250000) / 16384) / 10000;

    return (int32_t)tmp;
}

/**
 * @brief Convert raw pressure value to real pressure using calibration data.
 *
 * The sensor provides a raw pressure value. This function uses calibration data and the previously
 * compensated temperature to "compensate" (correct) the value, so it matches the real atmospheric
 * pressure.
 *
 * @param raw_pressure Raw pressure value from sensor
 * @return Compensated pressure in Pascals (Pa)
 */
int64_t bmp390CompensatePressure(uint32_t raw_pressure)
{
    /*
     * The following calculations are based on the official Bosch BMP3-Sensor-API.
     * Adapted from: https://github.com/BoschSensortec/BMP3-Sensor-API/blob/master/bmp3.c
     */
    int64_t partial_data1;
    int64_t partial_data2;
    int64_t partial_data3;
    int64_t partial_data4;
    int64_t partial_data5;
    int64_t partial_data6;
    int64_t offset;
    int64_t sensitivity;
    uint64_t comp_press;

    int64_t t_lin = compensated_t_for_p; // Use compensated temperature

    // Step-by-step calculation for pressure compensation
    partial_data1 = t_lin * t_lin;
    partial_data2 = partial_data1 / 64;
    partial_data3 = (partial_data2 * t_lin) / 256;
    partial_data4 = (calib_data.p8 * partial_data3) / 32;
    partial_data5 = (calib_data.p7 * partial_data1) * 16;
    partial_data6 = (calib_data.p6 * t_lin) * 4194304;
    offset = (calib_data.p5 * 140737488355328) + partial_data4 + partial_data5 + partial_data6;
    partial_data2 = (calib_data.p4 * partial_data3) / 32;
    partial_data4 = (calib_data.p3 * partial_data1) * 4;
    partial_data5 = (calib_data.p2 - 16384) * t_lin * 2097152;
    sensitivity =
        ((calib_data.p1 - 16384) * 70368744177664) + partial_data2 + partial_data4 + partial_data5;
    partial_data1 = (sensitivity / 16777216) * raw_pressure;
    partial_data2 = calib_data.p10 * t_lin;
    partial_data3 = partial_data2 + (65536 * calib_data.p9);
    partial_data4 = (partial_data3 * raw_pressure) / 8192;
    // The next two lines avoid overflow in multiplication
    partial_data5 = (raw_pressure * (partial_data4 / 10)) / 512;
    partial_data5 = partial_data5 * 10;
    partial_data6 = ((int64_t)raw_pressure * (int64_t)raw_pressure);
    partial_data2 = (calib_data.p11 * partial_data6) / 65536;
    partial_data3 = (partial_data2 * raw_pressure) / 128;
    partial_data4 = (offset / 4) + partial_data1 + partial_data5 + partial_data3;

    // Final pressure value in hundredths of Pascals, so divide by 100 to get Pascals
    comp_press = (((uint64_t)partial_data4 * 25) / (uint64_t)1099511627776);
    comp_press /= 100;

    return comp_press;
}

/**
 * @brief Initialize the BMP390 sensor for use.
 *
 * This function wakes up the sensor, resets it, checks its identity, sets up oversampling and data
 * rate, reads calibration data, and enables normal measurement mode.
 *
 * @return 0 on success, negative error code on failure
 */
int bmp390Init(void)
{
    int error       = 0;
    uint8_t chip_id = 0;
    uint8_t data[2] = {0};

    wakeupBmp();
    // Soft reset the sensor to ensure it starts from a known state
    data[0] = BMP390_REG_CMD;
    data[1] = BMP390_CMD_SOFT_RESET;
    error   = i2c_write_dt(&bmp_i2c, data, 2);
    if(error < 0)
    {
        logWarning("BMP390: Soft reset failed");
        return error;
    }

    k_msleep(10); // Wait for reset to complete
    wakeupBmp();
    // Check that the sensor is really a BMP390 by reading its chip ID
    data[0] = BMP390_REG_CHIP_ID;
    error   = i2c_write_read_dt(&bmp_i2c, data, 1, &chip_id, 1);

    if(error != 0 || chip_id != BMP390_CHIP_ID)
    {
        logWarning("BMP390: Invalid chip ID: 0x%02x");
        return -chip_id;
    }

    // Set oversampling (improves accuracy)
    data[0] = BMP390_REG_OSR;
    data[1] = BMP390_OSR_DEFAULT;
    error   = i2c_write_dt(&bmp_i2c, data, 2);
    if(error < 0)
    {
        logWarning("BMP390: Failed to set oversampling");
        return error;
    }

    // Set output data rate (how often sensor measures)
    data[0] = BMP390_REG_ODR;
    data[1] = BMP390_ODR_DEFAULT;
    error   = i2c_write_dt(&bmp_i2c, data, 2);
    if(error < 0)
    {
        logWarning("BMP390: Failed to set output data rate");
        return error;
    }

    // Read calibration data from sensor
    error = readCalibData();
    if(error < 0)
    {
        logWarning("BMP390: Failed to read calibration data");
        return error;
    }

    // Enable sensor in normal measurement mode
    data[0] = BMP390_REG_PWR_CTRL;
    data[1] = BMP390_PWR_CTRL_ENABLE;
    error   = i2c_write_dt(&bmp_i2c, data, 2);
    if(error < 0)
    {
        logWarning("BMP390: Failed to enable power control");
        return error;
    }

    logInfo("BMP390: Initialized successfully");

    return 0;
}

/**
 * @brief Read and return the current pressure and temperature from the BMP390 sensor.
 *
 * This function reads the raw sensor data, compensates it using calibration values,
 * and returns the real atmospheric pressure (in Pascals) and temperature (in °C × 100).
 *
 * @param pressure_pa Pointer to store the compensated pressure value (Pascals)
 * @param temperature Pointer to store the compensated temperature value (°C × 100)
 * @return 0 on success, negative error code on failure
 */
int bmp390ReadPressureAndTemperature(uint32_t *pressure_pa, int32_t *temperature)
{
    int error             = 0;
    uint8_t data[6]       = {0};
    uint32_t raw_temp     = 0;
    uint32_t raw_pressure = 0;

    wakeupBmp();

    // Read 6 bytes: 3 for pressure, 3 for temperature
    data[0] = BMP390_REG_DATA_0;
    error   = i2c_write_read_dt(&bmp_i2c, data, 1, data, 6);
    if(error < 0)
    {
        logWarning("BMP390: Failed to read pressure data");
        return error;
    }

    // Convert bytes to 24-bit raw values (little-endian)
    raw_pressure = (uint32_t)data[2] << 16 | (uint32_t)data[1] << 8 | data[0];
    raw_temp     = (uint32_t)data[5] << 16 | (uint32_t)data[4] << 8 | data[3];

    // Compensate raw values to get real temperature and pressure
    *temperature = bmp390CompensateTemp(raw_temp);
    *pressure_pa = bmp390CompensatePressure(raw_pressure);

    return 0;
}
