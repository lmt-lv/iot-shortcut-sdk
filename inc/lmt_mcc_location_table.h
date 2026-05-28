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
/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef MCC_LOCATION_TABLE_H_
#define MCC_LOCATION_TABLE_H_
#include <stdint.h>

/**
 * @brief Struct for mccTable entries.
*/
struct __attribute__((__packed__)) mccTable
{
    uint8_t confidence;    /* percentage, 0-100 */
    uint8_t unc_semiminor; /* scaled, see GNSS interface for details */
    uint8_t unc_semimajor; /* scaled, see GNSS interface for details */
    uint8_t orientation;   /* orientation angle between the major axis and north */
    float lat;
    float lon;
    uint16_t mcc;
};

/**
 * @brief Finds location information for a given Mobile Country Code (MCC).
 *
 * @param[in] mcc MCC to look for.
 *
 * @return Location information for the matching MCC, NULL if no entry was found.
 */
const struct mccTable *mccLookup(uint16_t mcc);

/**
 * @brief Converts a latitude in degrees to the integer representation used by GNSS.
 *
 * @param[in] lat Latitude in degrees.
 *
 * @return Latitude in scaled integer representation.
 */
int32_t latConvert(float lat);

/**
 * @brief Converts a longitude in degrees to the integer representation used by GNSS.
 *
 * @param[in] lon Longitude in degrees.
 *
 * @return Longitude in scaled integer representation.
 */
int32_t lonConvert(float lon);

#endif /* MCC_LOCATION_TABLE_H_ */
