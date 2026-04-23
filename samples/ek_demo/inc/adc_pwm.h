
/**
 * @file adc_pwm.h
 * @brief Interface for reading potentiometer position and controlling PWM output
 *
 * This header provides functions to:
 *   - Read the position of a potentiometer (knob) using the ADC (Analog-to-Digital Converter)
 *   - Run a background task that continuously reads the potentiometer and sets PWM output
 * accordingly
 *
 * The potentiometer position is always given as a percentage (0% to 100%).
 */

#ifndef ADC_PWM_H
#define ADC_PWM_H

/**
 * @brief Read the potentiometer position as a percentage (0-100%).
 *
 * This function measures the position of a knob (potentiometer) and converts it to a percentage
 * value.
 *
 * @param position Pointer to an unsigned integer where the result (percentage) will be stored.
 */
void readPotPosition(unsigned *position);

/**
 * @brief Task to read ADC value, calculate potentiometer position, and set PWM output.
 *
 * This function is intended to run as a background task. It repeatedly:
 *   - Reads the analog value from the potentiometer using the ADC
 *   - Calculates the position as a percentage (0-100%)
 *   - Sets the PWM output to match the potentiometer position
 *
 * The arguments (arg1, arg2, arg3) are not used.
 *
 * @param arg1 Unused
 * @param arg2 Unused
 * @param arg3 Unused
 */
void adcPwmTask(void *arg1, void *arg2, void *arg3);

#endif // ADC_PWM_H