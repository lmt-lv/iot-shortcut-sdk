/*
 * Copyright 2026 Latvijas Mobilais Telefons
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

#ifndef LMT_GNSS_ASSISTANCE_H_
#define LMT_GNSS_ASSISTANCE_H_

#include "lmt_mcc_location_table.h"
#include <nrf_modem_gnss.h>

/**
 * @brief Supplies rough location and time to the nRF GNSS module using mobile network data.
 * 
 * @param[in] agnss_request Flags from GNSS module which AGNSS data is request.
 * 
 * @return 0 on success.
*/
int assistanceRequest(struct nrf_modem_gnss_agnss_data_frame *agnss_request);

#endif // LMT_GNSS_ASSISTANCE_H_
