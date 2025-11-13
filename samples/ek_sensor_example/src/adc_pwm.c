/*
 * adc_pwm.c
 *
 * This file provides functions to:
 *   - Read the position of a potentiometer (knob) using the ADC (Analog-to-Digital Converter)
 *   - Set the PWM (Pulse Width Modulation) output based on the potentiometer position
 *
 * The potentiometer position is always given as a percentage (0% to 100%).
 *
 * Key steps and calculations are explained in comments.
 */

#include "adc_pwm.h"
#include "app_status_bits.h"     // For setting boot OK mask
#include "lmt_common.h"          // For common definitions
#include "lmt_storage_manager.h" // For log functions
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/pwm.h>

// PWM configuration: Use the device tree alias 'pwm_led0' for the PWM output
#define PWM_LED0 DT_ALIAS(pwm_led0)

// ADC and PWM device structures
// These hold configuration for the ADC channel and PWM output, using device tree macros
static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));
static const struct pwm_dt_spec pwm_led0    = PWM_DT_SPEC_GET(PWM_LED0);

// Buffer to store the raw ADC reading
static int32_t buf;
// ADC sequence configuration: tells the ADC driver where to put the result
struct adc_sequence sequence = {
    .buffer      = &buf,
    .buffer_size = sizeof(buf),
    // .calibrate = true, // Optional: enable calibration if needed
};

// The maximum expected ADC voltage in millivolts (mV). Setting a bit below the actual VDD to allow
// actual readings to set the maximum
static uint32_t max_adc_mv = 3525;
// The potentiometer position as a percentage (0-100%) of the maximum ADC value
static int adc_perc = 0;

/**
 * @brief Initialize the ADC (Analog-to-Digital Converter).
 *
 * Prepares the ADC hardware to read analog values from the potentiometer.
 *
 * @return 0 on success, negative error code on failure
 */
int initAdc(void)
{
    int error = 0;
    // Check if the ADC device is ready
    if(!adc_is_ready_dt(&adc_channel))
    {
        logWarning("ADC controller device not ready");
        return -ENODEV;
    }

    // Set up the ADC channel
    error = adc_channel_setup_dt(&adc_channel);
    if(error < 0)
    {
        logWarning("Could not setup channel");
        return error;
    }

    // Initialize the ADC sequence (where to store the result)
    error = adc_sequence_init_dt(&adc_channel, &sequence);
    if(error < 0)
    {
        logWarning("Could not initialize sequence");
        return error;
    }

    logInfo("ADC sequence initialized");

    return 0;
}

/**
 * @brief Initialize the PWM (Pulse Width Modulation) output.
 *
 * Prepares the PWM hardware to control an output (such as an LED or motor) based on the
 * potentiometer position.
 *
 * @return 0 on success, negative error code on failure
 */
int initPwm(void)
{
    int error = 0;
    // Check if the PWM device is ready
    if(!pwm_is_ready_dt(&pwm_led0))
    {
        logWarning("Error: PWM device is not ready");
        return -ENODEV;
    }

    logInfo("PWM device is ready");

    // Set initial PWM duty cycle to 0 (output off)
    error = pwm_set_dt(&pwm_led0, max_adc_mv, 0);
    if(error)
    {
        logWarning("Could not set PWM duty cycle");
    }

    return error;
}

/**
 * @brief Get the current potentiometer position as a percentage (0-100%).
 *
 * @param position Pointer to an unsigned integer where the result will be stored.
 */
void readPotPosition(unsigned *position)
{
    if(position != NULL)
    {
        *position = adc_perc;
    }
}

/**
 * @brief Task to read potentiometer position and set PWM output.
 *
 * This function runs in a loop:
 *   - Reads the analog value from the potentiometer using the ADC
 *   - Converts the value to millivolts (mV)
 *   - Calculates the position as a percentage (0-100%)
 *   - Sets the PWM output to match the potentiometer position
 *   - Waits for 1 second before repeating
 *
 * If the ADC or PWM initialization fails, an error is logged.
 *
 * @param arg1 Unused
 * @param arg2 Unused
 * @param arg3 Unused
 */
void adcPwmTask(void *arg1, void *arg2, void *arg3)
{
    int error = 0;
    int adc_error, pwm_error;
    // ADC value in millivolts
    int32_t adc_mv = 0;

    adc_error = initAdc();
    if(adc_error)
    {
        logError("ADC init failed", adc_error);
    }

    pwm_error = initPwm();
    if(pwm_error)
    {
        logError("PWM init failed", pwm_error);
    }

    // Only set the boot OK bit if both ADC and PWM were initialized successfully
    if(!adc_error && !pwm_error)
    {
        setBootOkBit(ADC_PWM_BOOT_OK_BIT);
    }

    while(1)
    {
        // Read ADC value (raw)
        error = adc_read(adc_channel.dev, &sequence);
        if(error < 0)
        {
            logError("Could not read ADC value", error);
            continue;
        }

        // Filter negative readings (can happen due to hardware effects)
        if((int16_t)buf < 0)
        {
            buf = 0;
        }

        // Store the raw ADC value
        adc_mv = buf;

        // Convert raw ADC value to millivolts (mV)
        error = adc_raw_to_millivolts_dt(&adc_channel, &adc_mv);
        if(error < 0)
        {
            logWarning(" (value in mV not available)");
        }

        // If the measured voltage is higher than expected, update the maximum (power supplies
        // aren't perfect)
        if(adc_mv > max_adc_mv)
        {
            max_adc_mv = adc_mv;
        }

        // Clamp negative values to zero (can happen due to hardware effects)
        if(adc_mv < 0)
        {
            adc_mv = 0;
        }

        // Calculate potentiometer position as a percentage of the maximum
        // Rounds up if halfway between two values
        adc_perc = ((adc_mv * 100) + (max_adc_mv / 2)) / max_adc_mv;

        // Uncomment to print potentiometer reading in percent
        // logInfoFormatted("Potentiometer value in %% of max: %d%%", adc_perc);

        // Set PWM output based on potentiometer position
        error = pwm_set_dt(&pwm_led0, max_adc_mv, adc_mv);
        if(error)
        {
            logError("Error in pwm_set_dt()", error);
            continue;
        }

        k_sleep(K_SECONDS(1)); // Wait for 1 second before next reading
    }
}