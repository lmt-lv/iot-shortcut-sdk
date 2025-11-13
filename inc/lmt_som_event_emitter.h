#ifndef LMT_SOM_EVENTS_EMITTER_H
#define LMT_SOM_EVENTS_EMITTER_H

#include <stdint.h>

/**
 * @brief Events emitted by the LMT SOM library.
 *
 * This enumeration defines all possible events that can be emitted by the library
 * to notify the application about various system and application-level occurrences,
 * such as initialization, uplink status, logging, and terminal commands.
 */
typedef enum
{
    EVENT_MAILER_INIT_OK, /**< Mailer module initialized successfully. */
    EVENT_PACKER_INIT_OK, /**< Packer module initialized successfully. */
    EVENT_LOGGER_INIT_OK, /**< Logger module initialized successfully. */
    EVENT_DEVICE_INIT_OK, /**< Device initialization completed successfully. */
    EVENT_UL_START,       /**< Uplink process started. */
    EVENT_UL_OK,          /**< Uplink completed successfully. */
    EVENT_UL_FAIL,        /**< Uplink failed. */
    EVENT_UL_RETRY,       /**< Uplink retry is being attempted. */
    EVENT_UL_QUEUE_FULL,  /**< Uplink queue is full. */
    EVENT_LOG_ERROR,      /**< An error log event occurred. */
    EVENT_LOG_WARNING,    /**< A warning log event occurred. */
    EVENT_LOG_INFO,       /**< An informational log event occurred. */
    EVENT_TERMINAL_CMD    /**< A terminal command event occurred. */
} SomEvent;

/**
 * @brief Centralized event handler for the library.
 * This is a weak function that can be overridden by the user application.
 *
 * @param event The event type.
 * @param data Optional data associated with the event (can be NULL).
 * @param data_len Length of the data (if applicable).
 */
void handleSomEvent(SomEvent event, void *data, uint8_t data_len);

#endif // LMT_SOM_EVENTS_EMITTER_H