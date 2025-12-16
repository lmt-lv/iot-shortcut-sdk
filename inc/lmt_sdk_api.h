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
