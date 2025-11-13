#ifndef LMT_PROTO_HANDLER_H
#define LMT_PROTO_HANDLER_H

#include "A2.pb.h"

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

/**
 * @brief Dumps in HEX the memory region at the given address of the given size
 *
 * @param *ptr The starting memory address
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
 * @brief Returns flag signalising that data is updated in comparison
 * to the last encoded message
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
 * @return Column count in Tape
 */
pb_size_t getTapeRecordsCount(uint8_t i_tape);

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
 * @brief Sets the uplink message uplink event
 *
 * @param event Uplink event type
 */
void setUplinkEvent(UplinkEventType event);

/**
 * @brief Encodes uplink message
 *
 * @return success flag
 */
bool encodeMessage(void);

/**
 * @brief Get encoded message length
 *
 * @return Current encoded message length
 */
uint16_t getEncodedMsgLen(void);

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

#endif // LMT_PROTO_HANDLER_H
