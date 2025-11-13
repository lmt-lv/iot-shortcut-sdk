#ifndef LMTSDK_API_H
#define LMTSDK_API_H

#include "lmt_coap_manager.h"
#include "lmt_common.h"
#include "lmt_proto_handler.h"
#include "lmt_settings.h"
#include "lmt_som_event_emitter.h"
#include "lmt_storage_manager.h"

/**
 * @brief Initializes the SDK.
 */
void lmtInit(void);

/**
 * @brief User application setup code
 */
void setup(void);

/**
 * @brief User application main control loop
 */
void loop(void);

#endif // LMTSDK_API_H
