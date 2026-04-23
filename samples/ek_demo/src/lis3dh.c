/*
 * lis3dh.c
 *
 * This file provides functions to:
 *   - Initialize the LIS3DH accelerometer sensor
 *   - Read the latest acceleration data from the LIS3DH sensor
 *   - Track and retrieve the maximum acceleration measured since the last reset
 *   - Run a background task that continuously reads and processes acceleration data
 *
 * Acceleration values are given in units of g (gravitational acceleration, where 1g ≈ 9.81 m/s²).
 *
 * The code is designed to be understandable for non-programmers. Key steps and calculations are
 * explained in comments.
 */

#include "lis3dh.h"
#include "app_status_bits.h"     // For setting boot OK bit
#include "lmt_common.h"          // For common definitions
#include "lmt_storage_manager.h" // For log functions
#include <math.h>
#include <zephyr/drivers/sensor.h>

// Pointer to the LIS3DH accelerometer device (configured via device tree)
static const struct device *lis3dh = DEVICE_DT_GET(DT_NODELABEL(lis3dh));
// Buffer to store acceleration data for X, Y, Z axes
static struct sensor_value accel[3] = {0};
// Maximum acceleration measured since last reset (in m/s^2)
static float max_accel = 0.0f;

/**
 * @brief Initialize the LIS3DH accelerometer sensor.
 *
 * Checks if the sensor is ready to use.
 *
 * @return 0 on success, negative error code on failure
 */
int initLis3dh(void)
{
    // Check if the LIS3DH device is ready to use
    if(!device_is_ready(lis3dh))
    {
        logWarning("LIS3DH device not ready");
        return -ENODEV;
    }

    // Initialization successful
    return 0;
}

/**
 * @brief Read the latest acceleration data from the LIS3DH sensor.
 *
 * Fetches a new sample and stores the X, Y, Z acceleration values in the accel array.
 */
void readLis3dh(void)
{
    int error = 0;

    // Fetch a new sample from the sensor
    error = sensor_sample_fetch(lis3dh);
    if(error)
    {
        logWarning("Failed to fetch LIS3DH sample");
    }

    // Get the X, Y, Z acceleration values
    error = sensor_channel_get(lis3dh, SENSOR_CHAN_ACCEL_XYZ, accel);
    if(error)
    {
        logWarning("Failed to get LIS3DH channel data");
    }

    return;
}

/**
 * @brief Get the maximum acceleration measured since the last reset.
 *
 * @param max Pointer to a float where the maximum acceleration (in g) will be stored.
 *            After reading, the stored maximum is reset to 0.
 */
void readLis3dhMax(float *max)
{
    if(max != NULL)
    {
        *max      = max_accel;
        max_accel = 0.0f; // Reset max after reading
    }
}

/**
 * @brief Task to continuously read LIS3DH sensor and track maximum acceleration.
 *
 * This function runs in a loop:
 *   - Reads the latest acceleration data (X, Y, Z axes)
 *   - Converts the values to floating-point numbers in m/s^2
 *   - Calculates the magnitude (modulus) of the acceleration vector
 *   - Updates the maximum value if a new peak is found
 *   - Waits 1 ms before repeating (for a 1 kHz data rate)
 *
 * The arguments (arg1, arg2, arg3) are not used.
 *
 * @param arg1 Unused
 * @param arg2 Unused
 * @param arg3 Unused
 */
void lis3dhTask(void *arg1, void *arg2, void *arg3)
{
    int error     = 0;
    float x       = 0;
    float y       = 0;
    float z       = 0;
    float modulus = 0;

    // Only set the boot OK bit if LIS3DH initialization succeeded
    error = initLis3dh();
    if(error)
    {
        logError("LIS3DH init failed, error: %d", error);
    }
    else
    {
        setBootOkBit(LIS3DH_BOOT_OK_BIT);
    }

    // Let the sensor stabilize for 100 ms after initialization
    k_sleep(K_MSEC(100));

    while(1)
    {
        readLis3dh(); // Read new data into accel array
        // Convert sensor values to floating-point (in m/s^2)
        x = accel[0].val1 + accel[0].val2 / 1000000.0f;
        y = accel[1].val1 + accel[1].val2 / 1000000.0f;
        z = accel[2].val1 + accel[2].val2 / 1000000.0f;
        // Calculate the magnitude of the acceleration vector
        modulus = sqrtf(x * x + y * y + z * z);

        // Update the maximum if this reading is higher
        if(modulus > max_accel)
        {
            max_accel = modulus;
            // Uncomment to print new maximum acceleration value
            // logInfoFormatted("New LIS3DH max acceleration: %.2f m/s^2", max_accel);
        }

        // Reset values for next iteration
        modulus = 0;
        x       = 0;
        y       = 0;
        z       = 0;
        // Sleep for 1 ms (1 kHz data rate)
        k_sleep(K_MSEC(1));
    }
}