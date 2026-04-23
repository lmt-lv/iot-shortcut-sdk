
/**
 * @file lis3dh.h
 * @brief Interface for reading acceleration from the LIS3DH sensor
 *
 * This header provides functions to:
 *   - Read the maximum acceleration measured by the LIS3DH sensor
 *   - Run a background task that continuously reads and stores the maximum acceleration
 *
 * Acceleration values are given in units of g (gravitational acceleration, where 1g ≈ 9.81 m/s²).
 *
 * These functions are designed to be easy to use and understand, even for those without a
 * programming background.
 */

#ifndef LIS3DH_H
#define LIS3DH_H

/**
 * @brief Get the maximum acceleration value measured by the LIS3DH sensor.
 *
 * This function returns the highest acceleration value (in m/s^2) measured since the last reset.
 *
 * @param max Pointer to a float where the maximum acceleration (in m/s^2) will be stored.
 */
void readLis3dhMax(float *max);

/**
 * @brief Task to read LIS3DH sensor, calculate maximum acceleration, and store it.
 *
 * This function is intended to run as a background task. It repeatedly:
 *   - Reads acceleration data from the LIS3DH sensor
 *   - Calculates the magnitude (modulus) of the acceleration vector
 *   - Stores the maximum value found
 *
 * The arguments (arg1, arg2, arg3) are not used.
 *
 * @param arg1 Unused
 * @param arg2 Unused
 * @param arg3 Unused
 */
void lis3dhTask(void *arg1, void *arg2, void *arg3);

#endif // LIS3DH_H