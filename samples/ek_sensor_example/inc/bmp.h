
/**
 * @file bmp.h
 * @brief Interface for BMP390 pressure and temperature sensor
 *
 * This header provides functions to initialize the BMP390 sensor and read accurate atmospheric
 * pressure and temperature values.
 *
 * All pressure values are in Pascals (Pa). All temperature values are in hundredths of degrees
 * Celsius (°C × 100).
 *
 * "Calibration data" refers to factory-set values stored in the sensor, used to correct
 * ("compensate") the raw readings for accuracy.
 */

#ifndef BMP_H
#define BMP_H

#include <stdint.h>

/**
 * @brief Initialize the BMP390 sensor for use.
 *
 * This function wakes up the sensor, resets it, checks its identity, sets up oversampling and data
 * rate, reads calibration data, and enables normal measurement mode.
 *
 * The calibration data is essential for converting raw sensor values into real-world units.
 *
 * @return 0 if successful, negative error code if there was a problem
 */
int bmp390Init(void);

/**
 * @brief Read the current atmospheric pressure and temperature from the BMP390 sensor.
 *
 * This function reads the sensor, applies calibration to correct the values, and returns the
 * results.
 *
 * @param pressure_pa Pointer to variable where pressure (in Pascals) will be stored
 * @param temperature Pointer to variable where temperature (in °C × 100) will be stored
 * @return 0 if successful, negative error code if there was a problem
 */
int bmp390ReadPressureAndTemperature(uint32_t *pressure_pa, int32_t *temperature);

#endif // BMP_H