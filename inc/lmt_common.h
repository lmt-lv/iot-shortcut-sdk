#ifndef LMT_COMMON_H
#define LMT_COMMON_H

#include <zephyr/kernel.h>
#include <zephyr/sys/__assert.h>

/** @cond INTERNAL_MACROS */
#define MAILER_BOOT_OK_BIT 0
#define PACKER_BOOT_OK_BIT 1
#define LOGGER_BOOT_OK_BIT 2
/** @endcond */

#define FIRST_USER_STATUS_BIT 16
#define LAST_USER_STATUS_BIT  31

/**
 * @brief Calls assert and triggers system reset in case of critical error.
 *
 */
void criticalError(void);

/**
 * @brief Returns the flag if the given mask is set in BootOK status
 *
 * @param mask The mask of the interested flags
 *
 * @return true if all flags are set, false otherwise
 */
bool checkBootOkMask(uint32_t mask);

/**
 * @brief Set user app boot ok mask
 *
 * @param mask The mask of the interested flags
 */
void setUserBootOkMask(uint32_t mask);

/**
 * @brief Set appropriate status bit
 *
 * @param bit The number of bit that has to be set
 */
void setBootOkBit(uint32_t bit);

/**
 * @brief Reset appropriate status bit
 *
 * @param bit The number of bit that has to be reset
 */
void resetStatusBit(uint32_t bit);

#endif // LMT_COMMON_H
