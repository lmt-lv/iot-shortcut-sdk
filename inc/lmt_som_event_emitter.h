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

#ifndef LMT_SOM_EVENTS_EMITTER_H
#define LMT_SOM_EVENTS_EMITTER_H

#include <stdint.h>

/**
 * @brief Events emitted by the IoT Shortcut SDK library.
 *
 * This enumeration defines all possible events that can be emitted by the library
 * to notify the application about various system and application-level occurrences,
 * such as initialization, uplink status, logging, and terminal commands.
 */
typedef enum
{
    EVENT_DEVICE_INIT_OK, /**< Device initialization completed successfully. */
    EVENT_LOGGER_INIT_OK, /**< Logger module initialized successfully. */
    EVENT_PACKER_INIT_OK, /**< Packer module initialized successfully. */
    EVENT_MAILER_INIT_OK, /**< Mailer module initialized successfully. */

    EVENT_DROPPING_OLDEST, /**< Uplink queue is full, the oldest message(s) are dropped. */

    EVENT_PACKER_STARTED, /**< Packer started. */
    EVENT_PACKING_FAILED, /**< Data packing failed. */
    EVENT_ENQUEUE_FAILED, /**< Packer failed to enqueue packet. */
    EVENT_PACKER_DONE_OK, /**< Package created and enqueued successfully. */

    EVENT_UL_START,     /**< Uplink process started. */
    EVENT_UL_MAX_RETRY, /**< Uplink retry limit reached. */
    EVENT_UL_RETRY,     /**< Uplink retry is being attempted. */
    EVENT_UL_DONE,      /**< Uplink completed. */

    EVENT_RRC_IDLE,      /**< RRC connection entered idle state. */
    EVENT_RRC_CONNECTED, /**< RRC connection entered connected state. */

    EVENT_MODEM_ON,  /**< Modem is actively consuming power. */
    EVENT_MODEM_OFF, /**< Modem is powered off to save energy. */

    EVENT_COAP_START, /**< COAP packet uplink started. */
    EVENT_COAP_FAIL,  /**< COAP packet uplink failed. */
    EVENT_COAP_NOACK, /**< No ACK received for the COAP packet. */
    EVENT_COAP_OK,    /**< COAP packet uplink completed successfully. */

    EVENT_LOG_ERROR,    /**< An error log event occurred. */
    EVENT_LOG_WARNING,  /**< A warning log event occurred. */
    EVENT_LOG_INFO,     /**< An informational log event occurred. */
    EVENT_TERMINAL_CMD, /**< A terminal command event occurred. */
    EVENT_COUNT         /**< Event count. */
} SomEvent;

typedef void (*EventHandler)(void *p_data, int i_data);

/**
 * @brief Centralized event handler for the library.
 * This is a weak function that can be overridden by the user application.
 *
 * @param event The event type.
 * @param p_data Optional data pointer associated with the event (can be NULL).
 * @param i_data Optional integer data associated with the event
 * if specified in the event description (e.g., length of the data).
 */
void handleSomEvent(SomEvent event, void *p_data, int i_data);

#endif // LMT_SOM_EVENTS_EMITTER_H