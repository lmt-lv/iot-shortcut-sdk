#ifndef LMT_SETTINGS_H
#define LMT_SETTINGS_H

#include <stdint.h>

/**
 * @brief Log level options for system logging.
 *
 * This enum defines the available log levels for controlling
 * the verbosity of saved system logs.
 */
typedef enum
{
    LOG_ERRORS = 0,
    LOG_WARNINGS,
    LOG_INFORMATIVE
} LogLevel;

/**
 * @brief Enum for active SIM control
 */
typedef enum
{
    ACTIVATE_ESIM = 0,
    ACTIVATE_SIM  = 1
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
 * @brief Set the device name used in COAP communication.
 *
 * @param name Null-terminated string representing the device name.
 */
void setCoapDeviceName(const char *name);

/**
 * @brief Set the pre-shared key (PSK) for COAPS communication.
 *
 * @param psk Null-terminated string representing the PSK.
 */
void setCoapServerPsk(const char *psk);

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
 * @brief Get the configured device name used in COAP.
 *
 * @return Null-terminated string representing the device name.
 */
const char *getCoapDeviceName(void);

/**
 * @brief Get the configured COAPS pre-shared key (PSK).
 *
 * @return Null-terminated string representing the PSK.
 */
const char *getCoapServerPsk(void);

/**
 * @brief Get the configured main COAPS server hostname.
 *
 * @return Null-terminated string with the server hostname.
 */
const char *getCoapServerHostname(void);

// Settings setter functions
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
 * @brief Set the CoAP response wait timeout.
 *
 * @param timeout Timeout for waiting for CoAP response in seconds.
 * 1 <= timeout <= 60.
 *
 * @return 0 on success, -EINVAL otherwise.
 */
int setResponseWaitTimeout(uint8_t timeout);

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
 * @brief Set the log level for the application.
 *
 * @param level Log level for the application.
 * LOG_ERRORS, LOG_WARNINGS, LOG_INFORMATIVE.
 *
 * @return 0 on success, -EINVAL otherwise.
 */
int setLogLevel(LogLevel level);

/**
 * @brief Sets the current SIM source
 *
 * @param sim_source Source of the SIM
 *
 * @return 0 on success, negative error code on failure.
 */
int setActiveSim(ActiveSim sim_source);

// Settings getter functions
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
uint16_t getLTEConnectionTimeout(void);

/**
 * @brief Get the device uplink timeout.
 *
 * @return Device uplink timeout in minutes.
 */
uint16_t getUplinkTimeout(void);

/**
 * @brief Get the device uplink timeout when no PSM available.
 *
 * @return Device uplink timeout when PSM not available in hours.
 */
uint16_t getNoPsmUplinkTimeout(void);

/**
 * @brief Get the initial timeout for resending packets.
 *
 * @return Initial timeout for resending packets in minutes.
 */
uint8_t getResendPacketInitialTimeout(void);

/**
 * @brief Get the maximum timeout for resending packets.
 *
 * @return Maximum timeout for resending packets in hours.
 */
uint8_t getMaxResendTimeout(void);

/**
 * @brief Get the maximum number of resend attempts.
 *
 * @return Maximum number of resend attempts before the socket is closed.
 */
uint8_t getMaxResendAttempts(void);

/**
 * @brief Get the frequency of log rotation checks.
 *
 * @return Frequency of log rotation checks in wakeup cycles.
 */
uint8_t getLogRotationFrequency(void);

/**
 * @brief Get the CoAP response wait timeout.
 *
 * @return Timeout for waiting for CoAP response in seconds.
 */
uint8_t getResponseWaitTimeout(void);

/**
 * @brief Get the number of retries for file uploads.
 *
 * @return Number of retries for file uploads.
 */
uint8_t getFileUlRetries(void);

/**
 * @brief Get the maximum number of log file count in flash.
 *
 * @return Number of maximum count of log files stored in flash.
 */
uint8_t getNumOfLogFiles(void);

/**
 * @brief Get the log level for the application.
 *
 * @return Log level for the application.
 */
LogLevel getLogLevel(void);

/**
 * @brief Returns the current SIM source
 *
 * @param sim_source Pointer to the source of the SIM
 *
 * @return 0 on success, negative error code on failure.
 */
int getActiveSim(ActiveSim *sim_source);

/**
 * @brief Disable serial logging output by suspending the UART device.
 *
 * Suspends the UART device using power management to disable serial output
 * for logging and debugging. This can help reduce power consumption when
 * serial output is not needed.
 *
 * @return 0 on success, negative error code on failure.
 *         Returns -EALREADY if the device is already suspended (not treated as error).
 */
uint8_t disableSerialLog(void);

/**
 * @brief Enable serial logging output by resuming the UART device.
 *
 * Resumes the UART device using power management to enable serial output
 * for logging and debugging. This allows real-time monitoring of device
 * operation after the UART was previously suspended.
 *
 * @return 0 on success, negative error code on failure.
 *         Returns -EALREADY if the device is already active (not treated as error).
 */
uint8_t enableSerialLog(void);

#endif // LMT_SETTINGS_H