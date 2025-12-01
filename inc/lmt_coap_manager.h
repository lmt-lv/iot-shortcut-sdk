#ifndef LMT_COAP_MANAGER_H
#define LMT_COAP_MANAGER_H

#include "lmt_common.h"
#include "lmt_proto_handler.h"

#define APP_COAP_MAX_MSG_LEN 1280

/** @cond INTERNAL_MACROS */
#define PACKER_LOOP_TIME 100 // Experimentally detected as 90+ ms
/** @endcond */

/**
 * @brief Enum for mailer wait control
 */
typedef enum
{
    WAIT_ON_TIMEOUT = 0,
    WAIT_FOREVER    = 1
} MailerWaitModes;

/**
 * @brief Reads device serial number (SN).
 *
 * @param sn_buffer Pointer to store the SN.
 * @param buffer_size Pointer to buffer size; updated with actual SN length.
 */
void getDeviceSN(char *sn_buffer, size_t *buffer_size);

/**
 * @brief Checks if the device is currently connected to the IoT network.
 *
 * @return true if the device is connected, false otherwise.
 */
bool isSocketConnected(void);

/**
 * @brief Checks if the modem is initialized.
 *
 * @return true if the modem is initialized, false otherwise.
 */
bool isModemInitialized(void);

/**
 * @brief Returns the mailer wait mode.
 *
 * @return WAIT_ON_TIMEOUT (false) if the mailer waits on timeout,
 * WAIT_FOREVER (true) if the mailer has to be started by user
 */
MailerWaitModes getMailerWaitMode(void);

/**
 * @brief Sets the mailer wait mode.
 *
 * @param mode WAIT_ON_TIMEOUT (false) if the mailer has to wait on timeout,
 * WAIT_FOREVER (true) if the user controls the mailer start
 */
void setMailerWaitMode(MailerWaitModes mode);

/**
 * @brief Retrieves the current packet counter value.
 *
 * @return Current packet counter value.
 */
uint32_t getPacketCounter(void);

/**
 * @brief Sets the packet counter to a specific value.
 *
 * @param counter The new value for the packet counter.
 */
void setPacketCounter(uint32_t counter);

/**
 * @brief Resets the packet counter to zero.
 */
void resetPacketCounter(void);

/**
 * @brief Retrieves the packet counter limit that will triger the radio data update.
 *
 * @return The maximum allowed packet counter value.
 */
uint32_t getPacketCounterLimit(void);

/**
 * @brief Sets a new packet counter limit that will triger the radio data update.
 *
 * @param limit The new upper limit for the packet counter.
 */
void setPacketCounterLimit(uint32_t limit);

/**
 * @brief Function gets the cached signal quality from the modem
 *
 * @param rsrp Pointer to store the RSRP value
 * @param rsrq Pointer to store the RSRQ value
 * @param snr Pointer to store the SNR value
 *
 * @return 0 if values values, -EINVAL otherwise
 */
int getNetworkQuality(int32_t *rsrp, int32_t *rsrq, int32_t *snr);

/**
 * @brief Enable packer thread to create a CoAP message
 *
 * @param set_include_radio_parms when set, radio data will be added
 */
void triggerDataPacking(bool set_include_radio_parms);

/**
 * @brief Enable mailer thread; first data packet containing radio data can be created
 *
 * @param trigger_radio_pata_packing when set, radio data packet will be added
 */
void triggerMailer(bool trigger_radio_pata_packing);

/** /// @private
 * @brief Function to set the firmware upgrade event
 *
 * @param success true if the upgrade was successful, false otherwise
 */
void setEventFwUpgradeEvent(bool success);

/** /// @private
 * @brief Set and send logs uploaded event
 */
void sendEventLogSent(void);

/** /// @private
 * @brief Sends a CoAP POST Block 1 request to the server
 *
 * @param filename The name of the file to send
 * @param data_chunk The data to send
 * @param size The size of data
 * @param total_size The total size of the file
 * @return 0 on success, negative error code on fail
 */
int sendFileChunk(const char *filename, const char *data_chunk, int size, int total_size);

/** /// @private
 * @brief Postpones the download and upgrade of the firmware
 *
 * @param p_upgrade_fw_path pointer to firmware file name received from server
 * @param length length of the firmware name
 */
void postponeUpgradeFw(uint8_t *p_upgrade_fw_path, uint8_t length);

/** /// @private
 * @brief Postpones the log read request
 */
void postponeLogRead();

/** /// @private
 * @brief Postpones terminal command execution
 *
 * @param p_cmd Pointer to the command
 * @param size Size of the command
 */
void postponeTerminalCmd(uint8_t *p_cmd, uint8_t size);

/** /// @private
 * @brief Returns the requested action; used to check if it is set.
 *
 * @return 0 if no action set, action value otherwise
 */
uint8_t getRequestedAction(void);

/**
 * @brief Function to close the socket and power off the modem
 */
void modemShutdown(void);

/**
 * @brief Set and send terminal command result
 *
 * @param res Result of the command, 0 if successful, negative value otherwise
 */
void sendEventCmdRes(int res);

/** /// @private
 * @brief CoAP thread main loop function
 *
 * @param arg1 Unused
 * @param arg2 Unused
 * @param arg3 Unused
 */
void packerThread(void *arg1, void *arg2, void *arg3);

/** /// @private
 * @brief Mailer thread main loop function
 *
 * @param arg1 Unused
 * @param arg2 Unused
 * @param arg3 Unused
 */
void mailerThread(void *arg1, void *arg2, void *arg3);

#endif // LMT_COAP_MANAGER_H
