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

// LMT IoT Shortcut SDK GNSS library
#ifndef LMT_GNSS_HANDLER_H
#define LMT_GNSS_HANDLER_H

#include "lmt_gnss_assistance.h"
#include <nrf_modem_gnss.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Wrapper enum for nrf modem GNSS dynamics mode.
 * 
 * Used to provide the nrf modem with the expected movement type of the device
 * to optimize fix acquisition time.
*/
typedef enum _GnssDynamicsMode
{
    GENERAL_PURPOSE = NRF_MODEM_GNSS_DYNAMICS_GENERAL_PURPOSE, /** General purpose mode. Assumed maximum speed 100 km/h. */
    STATIONARY = NRF_MODEM_GNSS_DYNAMICS_STATIONARY, /** Optimize for stationary use case. Assumed maximum speed 3 km/h. */
    PEDESTRIAN = NRF_MODEM_GNSS_DYNAMICS_PEDESTRIAN, /** Optimize for pedestrian use case. Assumed maximum speed 30 km/h. */
    AUTOMOTIVE = NRF_MODEM_GNSS_DYNAMICS_AUTOMOTIVE, /**  Optimize for automotive use case. Assumed maximum speed > 100 km/h */
} GnssDynamicsMode;

/**
 * @brief Wrapper type for nrf GNSS data frame
 * 
 * Used in the user-provided callback function to supply the obtained fix.
*/
typedef struct nrf_modem_gnss_pvt_data_frame GnssFix;


/**
 * @brief GNSS Fix obtained callback function prototype.
 * 
 * @param[in] gnss_data GNSS Fix obtained by the GNSS module.
*/
typedef void (*GnssFixHandler)(GnssFix *gnss_data);

/**
 * @struct
 * @brief Struct containing GNSS operation settings.
*/
typedef struct _GnssSettings
{
    /**
     * @brief Fix Interval is the time interval in seconds between location (fix)
     *        acquisition attempts.
     * @note This DOES NOT take into account how long the acquisition period
     *       (fix retry) is. For example if the fix retry value is 60 seconds
     *       and the fix interval is 120 seconds the guarranteed minimal sleep
     *       time (if no fix is found and fix retry runs out) is 60 seconds.
     *       This implies that the fix interval shall always be larger than
     *       the fix retry, and by a considerable margin (around 10 seconds).
     *       Equal or near equal fix interval and retry values can result
     *       (when no fix is found) in continous attempts to find a fix which
     *       is not recommended due to increased power consumption of the
     *       GNSS module.
    */
    uint16_t fix_interval;
    /**
     * @brief Fix Retry is the maximum time in seconds that will be spent to
     *        attempt to aquire a location(fix). For the first fix this will
     *        be 60 seconds and cannot be changed. Afterwards the supplied
     *        value is used.
    */
    uint16_t fix_retry;
    /**
     * @brief GNSS Dynamics mode - optimize GNSS module based on device usage.
    */
    GnssDynamicsMode dynamics_mode;
    /**
     * @brief Callback when GNSS location (fix) data is received.
    */
    GnssFixHandler callback;
} GnssSettings;

/**
 * @brief Start Acquiring GNSS location data.
 * 
 * @param[in] gnss_settings GNSS settings to apply to the GNSS module.
 * 
 * @return 0 on success.
*/
int startGnss(GnssSettings *gnss_settings);

/**
 * @brief  Stop Acquiring GNSS location data.
 * 
 * @return 0 on success.
*/
int stopGnss();

/**
 * @brief Check if the GNSS module has been enabled/started, disabled/stopped.
 * 
 * @return True when GNSS is enabled, false GNSS is disabled.
*/
bool getGnssEnabled();

/**
 * @brief Restart GNSS module with the settings supplied by startGnss.
 * 
 * @return 0 on success. -EPERM if no settings have been applied by startGnss.
*/
int restartGnss();

#endif
