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

#ifndef LMT_PROTO_HANDLER_H
#define LMT_PROTO_HANDLER_H

#include "telemetry.pb.h"

// Constants from .options file not defined in generated files
#define MAX_TRACKS_COUNT           12
#define MAX_PERIODS_COUNT          3
#define MAX_COLUMNS_COUNT          50
#define MAX_TAPE_COUNT             1
#define MAX_ACTION_PARAMETERS_SIZE 256
// Default tape index
#define I_TAPE 0

/** @cond INTERNAL_MACROS */
#define MAX_COAP_MESSAGE_HEAD_SIZE 30
#define FW_PATH_OFFSET             3
/** @endcond */

typedef enum
{
    EventSeverity_NOT_SPECIFIED = -1,
    EventSeverity_INFO,
    EventSeverity_WARNING,
    EventSeverity_ERROR,
    EventSeverity_CRITICAL
} EventSeverity;

_Static_assert((int)EventSeverity_INFO     == (int)CustomEventSeverity_INFO,     "severity out of sync");
_Static_assert((int)EventSeverity_WARNING  == (int)CustomEventSeverity_WARNING,  "severity out of sync");
_Static_assert((int)EventSeverity_ERROR    == (int)CustomEventSeverity_ERROR,    "severity out of sync");
_Static_assert((int)EventSeverity_CRITICAL == (int)CustomEventSeverity_CRITICAL, "severity out of sync");

/**
 * @brief Dumps in HEX the memory region at the given address of the given size
 *
 * @param ptr The starting memory address
 * @param size The size of the memory dump
 */
void dumpMemory(uint8_t *ptr, uint16_t size);

/** /// @private
 * @brief Initialises the Uplink UpMessage
 */
void initUpMessage(void);

/** /// @private
 * @brief Initialises the Downlink DoMessage
 */
void initDoMessage(void);

/** /// @private
 * @brief Initialises the protobuf handler
 */
void initProtobuf(void);

/**
 * @brief Returns flag signalising that the unencoded protobuf data
 * is changed since the last encoded message has been made
 *
 * @return A flag signalising that data had been changed
 */
bool isDataChanged(void);

/**
 * @brief Function to add a new period to the Periods array of a given Tape.
 * During operation first add the period, then measurement.
 * The duplicate value entries will be not added.
 * The entries withouth measurements will be overwritten.
 * On overflow it will reset the Periods array.
 * Corrects also the Columns_count if Columns_count >= MAX_COLUMNS_COUNT
 *
 * @param i_tape the Tape index
 * @param value the period value
 */
void updatePeriod(uint8_t i_tape, uint32_t value);

/**
 * @brief Returns the last defined period of the Tape instance given by pointer
 *
 * @param i_tape the Tape index
 * @return the last defined period or 0 if period is not set
 */
uint32_t getLastPeriod(uint8_t i_tape);

/**
 * @brief Adds a measuremens column and it period to the given Tape
 *
 * @param i_tape the Tape index
 * @param period the period value
 * @param p_measurements pinter to the measurements array in count of MAX_TRACKS_COUNT
 * @return number of remaining empty columns, -EINVAL if i_tape is out of range
 */
int addColumnToTape(uint8_t i_tape, uint32_t period, int32_t *p_measurements);

/**
 * @brief Returns actual column count in Tape
 *
 * @param i_tape the Tape index
 *
 * @return Column count in Tape, -EINVAL if i_tape is out of range
 */
int getTapeRecordsCount(uint8_t i_tape);

/**
 * @brief Resets the Tape keeping the last period entry
 *
 * @param i_tape the Tape index
 */
void rewindTape(uint8_t i_tape);

/**
 * @brief Clear sensor measurements keeping the last measuremet periods
 */
void restartMeasurements(void);

/** /// @private
 * @brief Returns the uplink message uplink event
 *
 * @return Server Initiated Action ID
 */
int getUplinkEvent(void);

/** /// @private
 * @brief Sets the uplink message uplink event
 *
 * @param event Uplink event type
 */
void setUplinkEvent(UplinkEventType event);

/** /// @private
 * @brief Returns the pointer to the created message Timestamp field
 *
 * @return pointer to the created message Timestamp field
 */
uint64_t *getPTimestamp(void);

/**
 * @brief Encodes uplink message.
 * Sets the out_message_length value. Resets f_data_changed flag.
 *
 * @return Zero on success, -EFAULT otherwise.
 */
int encodeMessage(void);

/**
 * @brief Get encoded message length
 *
 * @return Current encoded message length
 */
int getEncodedMsgLen(void);

/**
 * @brief Get pointer to encoded message buffer
 *
 * @return Constant pointer to encoded message buffer
 */
const uint8_t *getEncodedMsgBuffer(void);

/**
 * @brief Checks if the data will fit UDP packet with no compression
 *
 * @return true if data will not fit, false otherwise
 */
bool isCompressionCheckRequired(void);

/**
 * @brief Checks if the UDP packet is full
 *
 * @return true if full, false otherwise
 */
bool isUdpPacketFull(void);

/**
 * @brief Decodes downlink message
 *
 * @param p_buffer pointer to incoming message
 * @return success flag
 */
bool decodeMessage(const uint8_t *p_buffer);

/**
 * @brief Create and send custom event
 * 
 * @param event Custom event name. Max length 32 bytes including null terminator
 * @param value Custom event value string optional. Max length 32 bytes including null terminator NULL if not used
 * @param time Custom event timestamp optonal. 0 if timestamp is not used
 * @param severity Custom event severity level. Use NOT_SPECIFIED if not used
 * 
 * @return 0 on success
 */
int sendCustomEvent(const char *event, const char *value, uint64_t time, EventSeverity severity);

/**
 * @brief Adds the firmware version to the uplink message. It is sent once, in the first uplink.
 *
 * @param version Firmware version string. Max length 16 bytes including null terminator
 *
 * @return 0 on success, -EINVAL if the string is NULL, empty or too long
 */
int addFWVersionToUpMessage(const char *version);

#endif // LMT_PROTO_HANDLER_H