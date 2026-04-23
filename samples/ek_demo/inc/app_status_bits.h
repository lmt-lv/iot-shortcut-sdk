
/**
 * @file app_status_bits.h
 * @brief Defines status bits for tracking system startup success.
 *
 * This header provides macros to represent the startup status of different modules in the
 * application. Each module (main application, ADC/PWM, LIS3DH sensor) has a unique "boot OK" bit.
 * These bits are used to check if each part of the system started successfully.
 *
 * For example, if the ADC and PWM initialize correctly, ADC_PWM_BOOT_OK_BIT is set.
 *
 * This makes it easy to check system health and diagnose startup issues.
 */

#ifndef APP_STATUS_BITS_H
#define APP_STATUS_BITS_H

// Boot OK bits and values

// Each "BOOT_OK_BIT" is a unique number used to represent whether a part of the system started
// successfully. These bits are used to track the status of different modules during system startup.
//
// For example, if the ADC and PWM initialize correctly, ADC_PWM_BOOT_OK_BIT is set.

// Main application boot OK bit: set when the main application starts successfully
#define MAIN_BOOT_OK_BIT FIRST_USER_STATUS_BIT
// Value with only the MAIN_BOOT_OK_BIT set (used for status checking)
#define MAIN_BOOT_OK_VAL BIT(MAIN_BOOT_OK_BIT)

// ADC/PWM boot OK bit: set when ADC and PWM modules initialize successfully
#define ADC_PWM_BOOT_OK_BIT (FIRST_USER_STATUS_BIT + 1)
// Value with only the ADC_PWM_BOOT_OK_BIT set
#define ADC_PWM_BOOT_OK_VAL BIT(ADC_PWM_BOOT_OK_BIT)

// LIS3DH boot OK bit: set when the LIS3DH accelerometer initializes successfully
#define LIS3DH_BOOT_OK_BIT (FIRST_USER_STATUS_BIT + 2)
// Value with only the LIS3DH_BOOT_OK_BIT set
#define LIS3DH_BOOT_OK_VAL BIT(LIS3DH_BOOT_OK_BIT)

// APP_BOOT_OK is a combination of all the above values.
// If APP_BOOT_OK is set, it means all modules (main, ADC/PWM, LIS3DH) started successfully.
#define APP_BOOT_OK (MAIN_BOOT_OK_VAL | ADC_PWM_BOOT_OK_VAL | LIS3DH_BOOT_OK_VAL)

#endif // APP_STATUS_BITS_H