/*
 * Copyright 2025 Latvijas Mobilais Telefons
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
