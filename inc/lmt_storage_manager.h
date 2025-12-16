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

#ifndef LMT_STORAGE_MANAGER_H
#define LMT_STORAGE_MANAGER_H

#include <stdbool.h>
#include <stdio.h>

/** /// @private
 * @brief Wrapper function for calling rotateLogs()
 */
void timestampLogName(void);

/**
 * @brief Writes error to app.log file.
 *
 * @param text Error string.
 * @param code Error code.
 * @return 0 on success, negative value on fail
 */
int logError(char *text, int code);

/**
 * @brief Writes warning to app.log file.
 *
 * @param text Warning string.
 * @return 0 on success, negative value on fail
 */
int logWarning(char *text);

/**
 * @brief Logs the given buffer as hex string
 *
 * @param payload pointer to the buffer
 * @param message_length buffer size
 */
void logStringHex(uint8_t *payload, uint8_t message_length);

/**
 * @brief Variadic function for writing formatted info to app.log file.
 *
 * @param text info message string.
 * @return 0 on success, negative value on fail
 */
int logInfoFormatted(char *text, ...);

/**
 * @brief Function for writing informative string to app.log file.
 * Creating two separate functions because most of the time we don't
 * need to format the string.
 *
 * @param text info message string.
 * @return 0 on success, negative value on fail
 */
int logInfo(char *text);

/**
 * @brief Writes current device settings in JSON format to settings.txt file.
 *
 * @return 0 on success, negative value on fail
 */
int saveSettings(void);

/** /// @private
 * @brief Schedules all app_timestamp.log read.
 */
void appLogRead(void);

/**
 * @brief Schedules settings.txt read.
 */
void settingsFileRead(void);

/**
 * @brief Checks if firmware upgrade was successful or failed.
 * Detects successful upgrades by checking if running unconfirmed image.
 * Detects failed upgrades by checking for rejected images in secondary slot.
 *
 * @return 1 if firmware upgrade was successful, 0 if no upgrade occurred, -1 if upgrade failed
 */
int checkFwUpgradeStatus(void);

/**
 * @brief Wrapper function for filesystem flashErase()
 *
 * @return 0 on success, negative error code on fail
 */
int eraseFlash(void);

/** /// @private
 * @brief Saves the firmware chunk to the secondary image slot
 *
 * @param data Pointer to the fw chunk data
 * @param data_len Length of the data
 * @param last_block Flag to indicate if this is the last block
 *
 * @return 1 if last block saved and upgrade requested, 0 on success, negative value on fail
 */
int saveFwChunk(uint8_t *data, size_t data_len, bool last_block);

/** /// @private
 * @brief Logger thread main loop function
 *
 * @param arg1 Unused
 * @param arg2 Unused
 * @param arg3 Unused
 */
void loggerThread(void *arg1, void *arg2, void *arg3);

#endif // LMT_STORAGE_MANAGER_H