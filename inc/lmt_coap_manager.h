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
 * @brief Function to check if the modem is nRF9151
 *
 * @return true if the modem is nRF9151, false otherwise
 */
bool isNrf9151(void);

/**
 * @brief Function to check if the MCU revision supports NTN
 *
 * Reads the hardware identification, "AT%HWVERSION", and looks for
 * "LACA A1A". The model identification, "AT+CGMM", cannot answer this: it
 * reports the package variant without the silicon revision.
 *
 * @return true if the MCU revision supports NTN, false otherwise
 */
bool isNrfLacaA1A(void);

/**
 * @brief Function to check if the modem FW supports NTN
 *
 * Reads the firmware revision identification, "AT+CGMR", and looks for the
 * "-ntn" build-name suffix, as in "mfw_nrf9151-ntn".
 *
 * @return true if the modem FW supports NTN, false otherwise
 */
bool isThisFwNtn(void);

/**
 * @brief Checks whether the device is registered on a cell
 *
 * Reports the registration state as tracked from the LTE events, which is not
 * the same as reachability: a registered device asleep in PSM answers true, and
 * so does one whose CoAP socket is closed. Nor is it "the modem is up" - see
 * isModemInitialized(), which stays true at CFUN=4 with the radio off.
 *
 * @return true while the device is registered on a cell, false otherwise
 *
 * @see isSocketConnected(), isModemInitialized()
 */
bool isCellConnected(void);

/**
 * @brief Checks whether the CoAP socket to the server is open
 *
 * The socket survives a PSM sleep, since the PDN context stays active,
 * and it is closed only on failure and shutdown paths - a lost cell,
 * a send or receive failure, airplane mode, or the modem shutdown that
 * ends a no-PSM sleep. So a closed socket does not mean the device is
 * unregistered; for registration, see isCellConnected().
 *
 * @return true while the CoAP socket is open, false otherwise
 *
 * @see isCellConnected(), isModemInitialized()
 */
bool isSocketConnected(void);

/**
 * @brief Checks if the modem is initialized.
 *
 * @return true if the modem is initialized, false otherwise.
 */
bool isModemInitialized(void);

/**
 * @brief Reads device serial number (SN).
 *
 * @param sn_buffer Pointer to store the SN.
 * @param buffer_size Pointer to buffer size; updated with actual SN length.
 */
void getDeviceSN(char *sn_buffer, size_t *buffer_size);

/**
 * @brief Get the assigned PSM periodic TAU (Tracking Area Update) interval.
 *
 * Returns the periodic TAU value in effect, i.e. the 3GPP T3412 extended timer
 * (GPRS Timer 3 IE, 3GPP TS 24.008), in seconds. Read during modem
 * initialization via @c lte_lc_psm_get(), then tracked through
 * @c LTE_LC_EVT_PSM_UPDATE. This is what modem and network use; it may differ
 * from the value requested via @c getPsmReqTau().
 *
 * @return The assigned periodic TAU interval in seconds, or a negative error.
 * @retval -1 (@c INVALID_NETWORK_PARAM) No value available: PSM not granted,
 *            not supported, or unreadable from the modem.
 *
 * @see getPsmReqTau(), getPsmNetActiveTime()
 */
int getPsmNetTau(void);

/**
 * @brief Get the assigned PSM active time (3GPP T3324).
 *
 * What the network granted, read at modem initialization via
 * @c lte_lc_psm_get() and tracked through @c LTE_LC_EVT_PSM_UPDATE. Reported as
 * received: a granted value is always representable, so nothing is rounded.
 *
 * @return The assigned active time in seconds.
 * @retval -1 (@c INVALID_NETWORK_PARAM) Not granted, not supported, or
 *            unreadable from the modem.
 *
 * @see getPsmReqActiveTime(), getPsmNetTau(), getResponseWaitTimeout()
 */
int getPsmNetActiveTime(void);

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
 * @return 0 if values are correct, -EINVAL otherwise
 */
int getNetworkQuality(int32_t *rsrp, int32_t *rsrq, int32_t *snr);

/**
 * @brief Function returns the pointer to the cached Network RSRP value
 *
 * @return the pointer to the cached Network RSRP value
 */
int16_t *getPNetworkRsrp(void);

/**
 * @brief Function returns the pointer to the cached Network RSRQ value
 *
 * @return the pointer to the cached Network RSRQ value
 */
int16_t *getPNetworkRsrq(void);

/**
 * @brief Function returns the pointer to the cached Network SNR value
 *
 * @return the pointer to the cached Network SNR value
 */
int16_t *getPNetworkSnr(void);

/**
 * @brief Wake the packer thread to create a CoAP message from the queued records
 *
 * Returns as soon as the packer is signalled; the message is built on the
 * packer thread. Radio data is attached only when asked for, which costs a
 * modem query.
 *
 * @param set_include_radio_parms when set, radio data will be added
 *
 * @see triggerUpload(), which packs with radio data and then uploads
 */
void triggerPacking(bool set_include_radio_parms);

/**
 * @brief Wake the mailer thread to upload whatever is already queued
 *
 * Forms no new data packet. With nothing queued for upload it does nothing.
 * Returns as soon as the mailer is signalled, so the outcome of the upload is
 * not reported here - it arrives through the uplink and CoAP events.
 *
 * @see triggerUpload(), which packs a fresh radio-data packet first
 */
void triggerMailer(void);

/**
 * @brief Pack a fresh packet with radio data, then wake the mailer to upload
 *
 * Triggers the packer with the radio data flag set and waits for it to finish
 * before signalling the mailer, so the upload includes the new packet. In raw
 * data mode the packer is not triggerable and only the mailer is woken.
 *
 * Unlike the packing step, the upload itself is not awaited: this returns once
 * the mailer is signalled.
 *
 * @see triggerPacking(), triggerMailer()
 */
void triggerUpload(void);

/**
 * @brief Clean raw data pointer and length
 *
 * @return 0 on success, negative error code on fail
 */
int cleanRawData(void);

/**
 * @brief Set raw data pointer and length
 *
 * @param raw_data Pointer to raw data buffer
 * @param raw_data_len Length of the raw data
 * @param upload Flag to trigger mailer for immediate upload of raw data
 *
 * @return 0 on success, negative error code on fail
 */
int setRawData(uint8_t *raw_data, uint16_t raw_data_len, bool upload);

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
void postponeLogRead(void);

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