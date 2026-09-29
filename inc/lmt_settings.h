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

#ifndef LMT_SETTINGS_H
#define LMT_SETTINGS_H

#include <stdbool.h>
#include <stdint.h>

// Limits for CoAP acknowledgment wait timeout (in seconds)
#define COAP_ACK_WAIT_TIMEOUT_MAX 120

/**
 * @brief Log level options for system logging.
 *
 * This enum defines the available log levels for controlling
 * the verbosity of saved system logs.
 */
typedef enum
{
    LOG_DISABLED,
    LOG_ERRORS,
    LOG_WARNINGS,
    LOG_INFORMATIVE
} LogLevel;

/**
 * @brief Enum for active SIM control
 */
typedef enum
{
    ACTIVATE_ESIM = 0, // External SIM: SIM card
    ACTIVATE_SIM  = 1  // Onboard SIM chip
} ActiveSim;

// Debug helper
/**
 * @brief Prints coap structure.
 */
void printCoap(void);

// COAP key/value pair scaner
/**
 * @brief Scans a buffer for COAP key/value pairs stored as C strings (key\0value\0).
 *
 * @param p_coap_data a Pointer
 * @param size Size to scan.
 *
 * @return 0 on success, negative error code on failure.
 */
int scanForCoapKeys(const uint8_t *p_coap_data, unsigned int size);

// COAP setter functions
/**
 * @brief Set the COAP server port number.
 *
 * @param port COAP server port (typically 5683 for COAP, 5684 for COAPS).
 */
void setCoapServerPort(uint16_t port);

/**
 * @brief Set the COAP transmission resource path.
 *
 * @param resource Null-terminated string representing the resource path (e.g. "sensor/data").
 */
void setCoapTxResource(const char *resource);

/**
 * @brief Set the COAP file transmission resource path.
 *
 * @param resource Null-terminated string for the file resource (e.g. "upload").
 */
void setCoapTxFileResource(const char *resource);

/**
 * @brief Set the COAP firmware transmission resource path.
 *
 * @param resource Null-terminated string for the firmware update resource.
 */
void setCoapTxFwResource(const char *resource);

/**
 * @brief Set the hostname of the main COAPS server.
 *
 * @param hostname Null-terminated string with the server's hostname or IP address.
 */
void setCoapServerHostname(const char *hostname);

// COAP getter functions
/**
 * @brief Get the currently configured COAP server port.
 *
 * @return Server port as uint16_t.
 */
uint16_t getCoapServerPort(void);

/**
 * @brief Get the configured COAP transmission resource path.
 *
 * @return Null-terminated string representing the resource path.
 */
const char *getCoapTxResource(void);

/**
 * @brief Get the configured COAP file transmission resource path.
 *
 * @return Null-terminated string representing the file resource path.
 */
const char *getCoapTxFileResource(void);

/**
 * @brief Get the configured COAP firmware transmission resource path.
 *
 * @return Null-terminated string representing the firmware resource path.
 */
const char *getCoapTxFwResource(void);

/**
 * @brief Get the configured main COAPS server hostname.
 *
 * @return Null-terminated string with the server hostname.
 */
const char *getCoapServerHostname(void);

/**
 * @brief Get the configured device name used in COAP.
 *
 * @param device_name Pointer to a buffer where the device name will be stored.
 * @param len In: size of device_name. Out: serial number length. 0 on error
 */
void getCoapDeviceName(char *device_name, unsigned *len);

// Settings setter functions
/**
 * @brief Sets devices power option flag; it has to be set before the lmtInit().
 * By default the device counts as battery powered.
 *
 * @param battery_powered_flag power option flag: true for battery powered devices.
 */
void setPowerOption(bool battery_powered_flag);

/**
 * @brief Set the maximum size of the log file.
 *
 * @param size Maximum size of the log file in bytes.
 * 1024 (KB) <= size <= 1048576 (1MB).
 *
 * @return 0 on success, -EINVAL otherwise.
 */
int setLogFileMaxSize(int32_t size);

/**
 * @brief Set network connection timeout.
 *
 * @param timeout Device network connection timeout in seconds.
 * 1 (sec) <= timeout <= 1800 (30 min).
 *
 * @return 0 on success, -EINVAL otherwise.
 */
int setLTEConnectionTimeout(uint16_t timeout);

/**
 * @brief Set the device uplink timeout.
 *
 * @param timeout Device uplink timeout in minutes.
 * 5 (min) <= timeout <= 1440 (24h).
 *
 * @return 0 on success, -EINVAL otherwise.
 */
int setUplinkTimeout(uint16_t timeout);

/**
 * @brief Set the device uplink timeout when PSM not available.
 *
 * @param timeout Device uplink timout in hours
 * 1 <= timeout <= 24 (hours).
 *
 * @return 0 on success, -EINVAL otherwise.
 */
int setNoPsmUplinkTimeout(uint16_t timeout);

/**
 * @brief Set the initial timeout for resending packets.
 *
 * @param timeout Initial timeout for resending packets in minutes.
 * 1 (min) <= timeout <= 60 (1h).
 *
 * @return 0 on success, -EINVAL otherwise.
 */
int setResendPacketInitialTimeout(uint8_t timeout);

/**
 * @brief Set the maximum timeout for resending packets.
 *
 * @param timeout Maximum timeout for resending packets in hours.
 * 1 (h) <= timeout <= 24 (1d).
 *
 * @return 0 on success, -EINVAL otherwise.
 */
int setMaxResendTimeout(uint8_t timeout);

/**
 * @brief Set the maximum number of resend attempts.
 *
 * @param attempts Maximum number of resend attempts before the socket is closed.
 * 1 <= attempts <= 10.
 *
 * @return 0 on success, -EINVAL otherwise.
 */
int setMaxResendAttempts(uint8_t attempts);

/**
 * @brief Set the frequency of log rotation checks.
 *
 * @param frequency Frequency of log rotation checks in wakeup cycles.
 * 1 <= frequency <= 50.
 *
 * @return 0 on success, -EINVAL otherwise.
 */
int setLogRotationFrequency(uint8_t frequency);

/**
 * @brief Set the number of retries for file uploads.
 *
 * @param retries Number of retries for file uploads.
 * 1 <= retries <= 10.
 *
 * @return 0 on success, -EINVAL otherwise.
 */
int setFileUlRetries(uint8_t retries);

/**
 * @brief Set the maximum number of log files on flash before starting file rotation.
 *
 * @param file_count Number of log files on flash.
 * 1 <= file_count <= 20.
 *
 * @return 0 on success, -EINVAL otherwise.
 */
int setNumOfLogFiles(uint8_t file_count);

/**
 * @brief Set the severity level for log messages saved by the IoT Shortcut SDK.
 *
 * @param level Saved log message severity level. Valid values are:
 * LOG_ERRORS, LOG_WARNINGS, LOG_INFORMATIVE.
 *
 * @return 0 on success, -EINVAL otherwise.
 */
int setFileLogLevel(LogLevel level);

/**
 * @brief Sets the current SIM source
 *
 * @param sim_source Source of the SIM
 *
 * @return 0 on success, negative error code on failure.
 *
 * @note Depends on the existing of the "GPIO0" label in the DT for the GPIO port 0.
 */
int setActiveSim(ActiveSim sim_source);

/**
 * @brief Sets the current data upload mode
 *
 * @param mode true for raw data mode, false for normal mode
 */
void setUlDataMode(bool mode);

/**
 * @brief Set modem operating mode. Takes effect only when the modem library is next initialized.
 *
 * @param mode Set modem operating mode:
 * -1  LTE-M & NB-IoT support disabled,
 *  0  for LTE-M,
 *  1  for NB-IoT,
 *  2  for LTE-M & NB-IoT (default)
 *
 * @return 0 on success, negative error code on failure.
 */
int setModemMode(int mode);

/**
 * @brief Set modem LTE mode preference. Only has effect when the modem mode is 2 (LTE-M & NB-IoT),
 * and only when the modem library is next initialized.
 *
 * @param preference Set modem LTE mode preference
 *  0  no preference;
 *  1  LTE-M preferred (fallback to NB-IoT if LTE-M not available) (default);
 *  2  NB-IoT preferred (fallback to LTE-M if NB-IoT not available);
 *  3  Network priorities override system; prefer LTE-M when equal  priority.
 *  4  Network priorities override system; prefer NB-IoT when equal priority.
 *
 * @return 0 on success, negative error code on failure.
 */
int setLtePreference(uint8_t preference);

/**
 * @brief Sets or clears the LTE-M support flag in device settings. Takes effect only when the
 * modem library is next initialized.
 *
 * @param preference true to enable LTE-M, false to disable
 */
void setLtemFlag(bool preference);

/**
 * @brief Sets or clears the NB-IoT support flag in device settings. Takes effect only when the
 * modem library is next initialized.
 *
 * @param preference true to enable NB-IoT, false to disable
 */
void setNbiotFlag(bool preference);

/**
 * @brief Sets or clears the GNSS (GPS) support flag in device settings. Takes effect only when the
 * modem library is next initialized.
 *
 * @param preference true to enable GNSS, false to disable
 */
void setGnssFlag(bool preference);

/**
 * @brief Sets or clears the NTN NB-IoT support flag in device settings. Takes effect only when the
 * modem library is next initialized.
 *
 * @note NTN is mutually exclusive with the other services: enabling it
 * clears the modem mode, GNSS flag, and LTE preference.
 *
 * @param preference true to enable NTN NB-IoT, false to disable
 *
 * @retval 0  Success.
 * @retval 1  System lacks NTN modem firmware.
 * @retval 2  System lacks the NTN-capable MCU revision, LACA A1A.
 * @retval 3  System lacks both the LACA A1A revision and NTN modem firmware.
 *
 * @see isThisFwNtn(), isNrfLacaA1A()
 */
int setNtnFlag(bool preference);

/**
 * @brief Sets or clears the "keep the modem on" flag in device settings
 *
 * When set, losing the connection only closes the socket and the modem is left
 * powered in its current functional mode. When clear, the modem is additionally
 * driven offline via @c lte_lc_offline() and GNSS is stopped.
 *
 * @param val true to keep the modem on across a disconnect, false to let it go offline
 */
void setKeepModemOnFlag(bool val);

/**
 * @brief Set the requested PSM periodic TAU (Tracking Area Update) interval.
 *
 * Stores the RP-TAU interval to request from the network, i.e. the 3GPP T3412
 * extended timer (GPRS Timer 3 IE, 3GPP TS 24.008). It is applied via
 * @c lte_lc_psm_param_set_seconds() the next time the modem is initialized;
 * the value actually granted may differ and is read with @c getPsmNetTau().
 *
 * @param tau_interval The requested TAU interval in seconds. Stored verbatim -
 *                     neither rounded nor checked against the T3412 extended
 *                     encoding; when the modem is configured then the different
 *                     value can be used instead.
 *
 * @return 0 on success, or -EINVAL if @p tau_interval is negative.
 *
 * @see getPsmReqTau(), getPsmNetTau(), setPsmReqActiveTime()
 */
int setPsmReqTau(int tau_interval);

/**
 * @brief Set the requested PSM active time.
 *
 * Stores the active time to request from the network, i.e. the 3GPP T3324
 * timer (GPRS Timer 2 IE, 3GPP TS 24.008). It is applied via
 * @c lte_lc_psm_param_set_seconds() the next time the modem is initialized;
 * the value actually granted may differ and is read with
 * @c getPsmNetActiveTime().
 *
 * It also bounds the server ACK wait through @c getResponseWaitTimeout(),
 * which caps at @c COAP_ACK_WAIT_TIMEOUT_MAX, so a value below the granted
 * active time shortens that wait.
 *
 * @param timeout_seconds The requested active time in seconds. Stored verbatim
 *                        and checked only for being non-negative; the T3324
 *                        encoding rounds up later, so @c getPsmReqActiveTime()
 *                        may report more - anything in 63..119 becomes 120 -
 *                        and returns -EINVAL above 11160.
 *
 * @return 0 on success, or -EINVAL if @p timeout_seconds is negative.
 *
 * @see getPsmReqActiveTime(), getPsmNetActiveTime(), setPsmReqTau(),
 *      getResponseWaitTimeout().
 */
int setPsmReqActiveTime(int timeout_seconds);

/**
 * @brief Set the severity level for serial log output in the IoT Shortcut SDK.
 *
 * @param level Serial log message severity level. Valid values are:
 * LOG_DISABLED, LOG_ERRORS, LOG_WARNINGS, LOG_INFORMATIVE.
 *
 * @return 0 on success, -EINVAL otherwise.
 */
int setSerialLogLevel(LogLevel level);

/**
 * @brief Set the band sweep timeout value. Takes effect only when the modem library is next
 * initialized.
 *
 * @param timeout Band sweep timeout in seconds 10 <= timeout <= 255
 * @return 0 on success, -EINVAL if value is invalid
 */
int setPerBandTimeout(uint8_t timeout);

// Settings getter functions
/**
 * @brief Reads devices power option flag: true for battery powered devices.
 * By default the device counts as battery powered (this function returns true).
 *
 * @return True when the device is battery powered, false otherwise
 */
bool isDeviceBatteryPowered(void);

/**
 * @brief Get the maximum size of the log file.
 *
 * @return Maximum size of the log file in bytes.
 */
int32_t getLogFileMaxSize(void);

/**
 * @brief Get network connection timeout.
 *
 * @return Device network connection timeout in seconds.
 */
int getLTEConnectionTimeout(void);

/**
 * @brief Get the device uplink timeout.
 *
 * @return Device uplink timeout in minutes.
 */
int getUplinkTimeout(void);

/**
 * @brief Get the device uplink timeout when no PSM available.
 *
 * @return Device uplink timeout when PSM not available in hours.
 */
int getNoPsmUplinkTimeout(void);

/**
 * @brief Get the initial timeout for resending packets.
 *
 * @return Initial timeout for resending packets in minutes.
 */
int getResendPacketInitialTimeout(void);

/**
 * @brief Get the maximum timeout for resending packets.
 *
 * @return Maximum timeout for resending packets in hours.
 */
int getMaxResendTimeout(void);

/**
 * @brief Get the maximum number of resend attempts.
 *
 * @return Maximum number of resend attempts before the socket is closed.
 */
int getMaxResendAttempts(void);

/**
 * @brief Get the frequency of log rotation checks.
 *
 * @return Frequency of log rotation checks in wakeup cycles.
 */
int getLogRotationFrequency(void);

/**
 * @brief Get the CoAP response wait timeout.
 *
 * The smaller of the assigned and requested PSM active times, capped at
 * @c COAP_ACK_WAIT_TIMEOUT_MAX. Changes during a session; read on demand.
 *
 * @return Wait timeout in seconds.
 *
 * @see setPsmReqActiveTime(), getPsmNetActiveTime()
 */
int getResponseWaitTimeout(void);

/**
 * @brief Get the number of retries for file uploads.
 *
 * @return Number of retries for file uploads.
 */
int getFileUlRetries(void);

/**
 * @brief Get the maximum number of log file count in flash.
 *
 * @return Number of maximum count of log files stored in flash.
 */
int getNumOfLogFiles(void);

/**
 * @brief Get the log level for the application.
 *
 * @return Log level for the application.
 */
LogLevel getFileLogLevel(void);

/**
 * @brief Returns the current SIM source
 *
 * @param sim_source Pointer to the source of the SIM
 *
 * @return 0 on success, negative error code on failure.
 *
 * @note Depends on the existing of the "GPIO0" label in the DT for the GPIO port 0.
 */
int getActiveSim(ActiveSim *sim_source);

/**
 * @brief Get the current data upload mode
 *
 *  @return Data upload mode - 0 for normal mode, 1 for raw mode (unformatted data)
 */
int getUlDataMode(void);

/**
 * @brief Get modem operating mode
 *
 * @return 0 on success, negative error code on failure:
 * -1  LTE-M & NB-IoT support disabled,
 *  0  for LTE-M,
 *  1  for NB-IoT,
 *  2  for LTE-M & NB-IoT
 */
int getModemMode(void);

/**
 * @brief Get modem LTE mode preference. Has effect only if modem mode is set to 2 (LTE-M & NB-IoT).
 *
 * @return  Modem LTE mode preference
 *  0  no preference;
 *  1  LTE-M preferred (fallback to NB-IoT if LTE-M not available);
 *  2  NB-IoT preferred (fallback to LTE-M if NB-IoT not available);
 *  3  Network priorities override system; prefer LTE-M when equal  priority.
 *  4  Network priorities override system; prefer NB-IoT when equal priority.
 */
int getLtePreference(void);

/**
 * @brief Check whether LTE-M is enabled in device settings
 *
 * @return True when LTE-M is enabled, false otherwise
 *
 * @see setLtemFlag()
 */
bool isLtemEnabled(void);

/**
 * @brief Check whether NB-IoT is enabled in device settings
 *
 * @return True when NB-IoT is enabled, false otherwise
 *
 * @see setNbiotFlag()
 */
bool isNbiotEnabled(void);

/**
 * @brief Check whether GNSS (GPS) is enabled in device settings
 *
 * Reports the configured preference, not the receiver state - for whether the
 * receiver is currently running see @c isGnssActive().
 *
 * @return True when GNSS is enabled, false otherwise
 *
 * @see setGnssFlag(), isGnssActive()
 */
bool isGnssEnabled(void);

/**
 * @brief Check whether NTN NB-IoT is enabled in device settings
 *
 * @return True when NTN NB-IoT is enabled, false otherwise
 *
 * @see setNtnFlag()
 */
bool isNtnEnabled(void);

/**
 * @brief Check whether the mailer start is currently suspended
 *
 * Note the polarity: true means the mailer start is held back, as armed by
 * @c disableMailerStart().
 *
 * @return True while the mailer start is suspended, false otherwise
 *
 * @see disableMailerStart(), enableMailerStart()
 */
bool isMailerStartSuspended(void);

/**
 * @brief Check whether the modem is kept on across a disconnect
 *
 * @return True when the modem is left powered, false when it is driven offline
 *
 * @see setKeepModemOnFlag()
 */
bool isKeepModemOn(void);

/**
 * @brief Get the requested PSM periodic TAU (Tracking Area Update) interval.
 *
 * Returns the RP-TAU value requested from the network, i.e. the 3GPP T3412
 * extended timer (GPRS Timer 3 IE, 3GPP TS 24.008), in seconds. It is applied
 * to the modem during initialization via @c lte_lc_psm_param_set_seconds();
 * @c getPsmNetTau() reports what is actually in use.
 *
 * @return The requested PSM periodic TAU interval in seconds. Never negative;
 *         @c setPsmReqTau() rejects negative values. Defaults to 2400 seconds.
 *
 * @see getPsmNetTau(), setPsmReqTau(), getPsmReqActiveTime()
 */
int getPsmReqTau(void);

/**
 * @brief Get the requested PSM active time (3GPP T3324).
 *
 * What @c setPsmReqActiveTime() stored, reported verbatim. Applied at modem
 * initialization via @c lte_lc_psm_param_set_seconds(), which rounds it up to a
 * representable value; @c getPsmNetActiveTime() reports what was granted.
 *
 * @return The requested active time in seconds, 6 by default. Never negative -
 *         @c setPsmReqActiveTime() rejects negative values.
 *
 * @see getPsmNetActiveTime(), setPsmReqActiveTime(), getPsmReqTau()
 */
int getPsmReqActiveTime(void);

/**
 * @brief Get the current serial log level.
 *
 * @return IoT Shortcut SDK serial log level for the serial output.
 */
LogLevel getSerialLogLevel(void);

/**
 * @brief Get the band sweep timeout value.
 *
 * @return Band sweep timeout in seconds.
 */
int getPerBandTimeout(void);

// Enable/Disable couples

/**
 * @brief Suspends the mailer start:
 * arms a one-time delay of the mailer start by setting the specific flag.
 */
void disableMailerStart(void);

/**
 * @brief Resumes the mailer start:
 * releases the one-time mailer-start delay armed by disableMailerStart().
 */
void enableMailerStart(void);

#endif // LMT_SETTINGS_H