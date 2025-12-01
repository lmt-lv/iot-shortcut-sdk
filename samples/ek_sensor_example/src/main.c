/**
 * @file main.c
 * @brief Main application file for LMT SDK sample
 *
 * This file contains the main logic for reading sensor data and sending data to a server.
 *
 * Key steps and calculations are explained in comments.
 */

#include "adc_pwm.h"         // For reading potentiometer and controlling PWM
#include "app_status_bits.h" // For tracking system startup status
#include "bmp.h"             // For reading pressure and temperature from BMP390 sensor
#include "lis3dh.h"          // For reading acceleration from LIS3DH sensor
#include "lmt_sdk_app.h"     // Main application header (includes core SDK and command handler)

// Thread priorities: lower number = higher priority
#define PRIORITY_LIS3DH  1 // High priority for accelerometer task
#define PRIORITY_ADC_PWM 6 // Lower priority for ADC/PWM task

// Indices for storing sensor data in the Data array
#define BRIGHTNESS_INDEX   0 // Potentiometer (knob) position
#define TEMPERATURE_INDEX  1 // Temperature from BMP390
#define PRESSURE_INDEX     2 // Pressure from BMP390
#define ACCELARATION_INDEX 3 // Maximum acceleration from LIS3DH

#define DATA_READ_PERIOD 300 // Data read period in seconds (5 minutes)

// Define stack memory for the threads (required by Zephyr)
K_THREAD_STACK_DEFINE(lis3dh_stack_area, 1024);  // Stack for LIS3DH thread
K_THREAD_STACK_DEFINE(adc_pwm_stack_area, 1024); // Stack for ADC/PWM thread

// Thread data structures (used to manage threads)
static struct k_thread lis3dh_thread_data;
static struct k_thread adc_pwm_thread_data;

// This function is required to avoid linker errors (even if not used directly)
void force_link_k_msgq_put(struct k_msgq *q, void *data, k_timeout_t t)
{
    k_msgq_put(q, data, t);
}

/**
 * @brief Application setup/init code
 *
 * This function sets up the system, initializes BMP390 sensor, and starts background tasks
 * (threads). It also sets a status bit to indicate the main application started successfully.
 */
void appInit(void)
{
    int err = 0;

    // Set the boot OK mask: when all required modules are initialized,
    // the system will generate an EVENT_DEVICE_INIT_OK event.
    setUserBootOkMask(APP_BOOT_OK);

    // Initialize the BMP390 sensor (for pressure and temperature)
    err = bmp390Init();
    if(err)
    {
        logError("BMP390 init failed, err: %d", err);
        return;
    }

    // Start the LIS3DH accelerometer task in its own thread
    k_thread_create(&lis3dh_thread_data, lis3dh_stack_area,
                    K_THREAD_STACK_SIZEOF(lis3dh_stack_area), lis3dhTask, NULL, NULL, NULL,
                    PRIORITY_LIS3DH, 0, K_NO_WAIT);

    // Start the ADC/PWM task in its own thread
    k_thread_create(&adc_pwm_thread_data, adc_pwm_stack_area,
                    K_THREAD_STACK_SIZEOF(adc_pwm_stack_area), adcPwmTask, NULL, NULL, NULL,
                    PRIORITY_ADC_PWM, 0, K_NO_WAIT);

    // Indicate that the main application has started successfully
    setBootOkBit(MAIN_BOOT_OK_BIT);
}

/**
 * @brief Event callback functions
 *
 * The SDK uses an event-driven architecture where each SOM event has a corresponding callback
 * function. Users implement only the callbacks they need to handle specific events.
 *
 * Event naming pattern: EVENT_DEVICE_INIT_OK -> onDeviceInitOk (remove "EVENT_" prefix, camelCase)
 * Full list of available events is in `lmt_som_event_emitter.h`.
 *
 * To handle an event, simply implement its callback function with the signature:
 *   void onEventName(void *p_data, int i_data)
 *
 * Unimplemented callbacks are ignored by the SDK.
 */
void onDeviceInitOk(void *p_data, int i_data)
{
    logInfo("All system parts initialized successfully");
}

void onTerminalCmd(void *p_data, int i_data)
{
    // Perform the requested command and send the result
    sendEventCmdRes(runTerminalCmd(p_data, i_data));
}

/**
 * @brief Read sensor data and record it for transmission.
 *
 * This function reads all sensor values (potentiometer, temperature, pressure, acceleration),
 * stores them in an array, and prepares them for sending to the server.
 *
 * The data is packed and added to a buffer ("tape"). When the buffer is full, the data is sent or
 * when uplink timeout occurs.
 */
void readAndRecordData(void)
{
    int error                      = 0;
    int32_t Data[MAX_TRACKS_COUNT] = {0}; // Array to hold all sensor data
    uint32_t pressure              = 0;
    int32_t temperature            = 0;
    float accelaration_max         = 0;
    unsigned brightness            = 0;

    // Read temperature and pressure from BMP390 sensor. Temperature is in °C x 100, pressure in Pa.
    error = bmp390ReadPressureAndTemperature(&pressure, &temperature);
    if(error)
    {
        logError("BMP390 read failed, err: %d", error);
    }

    // Read maximum acceleration from LIS3DH sensor
    readLis3dhMax(&accelaration_max);
    // Read potentiometer (knob) position as a percentage
    readPotPosition(&brightness);

    // Print all read values
    logInfoFormatted("Pot: %d%%, Temp: %d.%02d C, Pressure: %d Pa, Accel max: %.2f m/s^2",
                     brightness, temperature / 100, temperature % 100, pressure, accelaration_max);

    // Store sensor data in the Data array
    Data[BRIGHTNESS_INDEX]  = brightness;  // Potentiometer position (0-100%)
    Data[TEMPERATURE_INDEX] = temperature; // Temperature (°C x 100)
    Data[PRESSURE_INDEX]    = pressure;    // Pressure (Pa)
    // Multiply acceleration by 100 to keep two decimal places
    Data[ACCELARATION_INDEX] = (int32_t)(accelaration_max * 100); // Acceleration (m/s^2 x 100)

    // Add the data to the tape (buffer for sending)
    error = addColumnToTape(I_TAPE, DATA_READ_PERIOD, Data);
    if(error == 0)
    {
        // If the buffer is full, trigger data packing (prepares data for sending)
        triggerDataPacking(false); // No radio data
    }
    else if(error < 0)
    {
        logError("Could not add measurement to tape", error);
        return;
    }
}

/**
 * @brief Main application entry point
 *
 * This is where the program starts running. It initializes the SDK and user application,
 * then enters a loop to periodically read sensor data and record it for transmission.
 *
 * The loop waits for a fixed period (DATA_READ_PERIOD), reads all sensor data, and repeats.
 *
 * @return Returns error code (should never return in normal operation)
 */
int main(void)
{
    int64_t start_tick = 0;
    int64_t end_tick   = 0;

    // Set uplink period in minutes(how often data is sent to server)
    setUplinkTimeout(5);
    // Initialize the LMT SDK (sets up core system functions)
    lmtInit();
    // Initialize the user application (sensors, threads, etc.)
    appInit();

    // Main loop: read and record data every DATA_READ_PERIOD seconds
    while(1)
    {
        // Wait for the next data read period (adjusted for time spent processing)
        k_sleep(K_SECONDS(DATA_READ_PERIOD - (end_tick - start_tick) / 1000));

        start_tick = k_uptime_get();
        readAndRecordData();
        end_tick = k_uptime_get();
    }

    return 0;
}
