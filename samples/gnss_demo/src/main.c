/**
 * @file main.c
 * @brief Main application file for LMT SDK sample
 *
 * This file contains a sample for setting up, starting and handling the GNSS module using
 * the IoT Shortcut SDK.
 *
 */

#include "lmt_sdk_app.h"


/**
 * @brief GNSS fix handler.
 *
 * Is called when a GNSS fix is acquired. Data from the fix is printed to the log.
 * 
 * @param[out] gnssOutput struct containing acquired GNSS fix data.
 */
void handleGnssFix(GnssFix *gnssOutput)
{
    logInfoFormatted("%02u:%02u:%02u,%.06f,%.06f,%.06f", gnssOutput->datetime.hour,
                     gnssOutput->datetime.minute, gnssOutput->datetime.seconds,
gnssOutput->latitude, gnssOutput->longitude, gnssOutput->accuracy
                    );

}

/**
 * @brief Application setup/init code
 *
 * This function sets up the system, defines the GNSS settings and starts GNSS acquisition.
 */
void appInit(void)
{
    enableSerialLog();


    GnssSettings gnss_settings = {
        .fix_interval       = 120,
        .fix_retry          = 60,
        .dynamics_mode      = STATIONARY,
        .callback = handleGnssFix,
    };
    startGnss(&gnss_settings);
}

/**
 * @brief Event callback functions
 *
 * The SDK uses an event-driven architecture where each SOM event has a corresponding callback
 * function. Users implement only the callbacks they need to handle specific events.
 *
 * Event naming pattern: EVENT_DEVICE_INIT_OK -> onDeviceInitOk (remove "EVENT_" prefix, camelCase)
 * Full list of available events is in `lmt_som_event_emitter.h`.
 *
 * To handle an event, simply implement its callback function with the signature:
 *   void onEventName(void *p_data, int i_data)
 *
 * Unimplemented callbacks are ignored by the SDK.
 */
void onDeviceInitOk(void *p_data, int i_data)
{
    logInfo("All system parts initialized successfully");
}


void onTerminalCmd(void *p_data, int i_data)
{
    // Perform the requested command and send the result
    sendEventCmdRes(runTerminalCmd(p_data, i_data));
}

/**
 * @brief Main application entry point
 *
 * This is where the program starts running. It initializes the SDK and user application,
 * then enters a loop.
 *
 * @return Returns error code (should never return in normal operation)
 */
int main(void)
{
    // Set uplink period in minutes(how often data is sent to server)
    setUplinkTimeout(5);
    // Initialize the LMT SDK (sets up core system functions)
    lmtInit();
    // Initialize the user application.
    appInit();

    while(1)
    {
        k_sleep(K_FOREVER);
    }

    return 0;
}
