/**
 * @file main.c
 * @brief Main application file for LMT SDK sample
 *
 * This file contains a sample for setting up, starting and handling the GNSS module using
 * the IoT Shortcut SDK.
 *
 */
#include "lmt_sdk_app.h"
#include <modem/modem_info.h>

#define CELL_ID_TRACK_ID 0
#define FIX_INTERVAL_S 60
#define FIX_RETRY_S 45

static struct modem_param_info modem_param_info;

/**
 * @brief GNSS fix handler.
 *
 * Is called when a GNSS fix is acquired. Data from the fix is printed to the log.
 * The current cell id and the fix data is sent to the LMT cloud.
 * 
 * @param[in] gnssOutput Pointer to a structure containing acquired GNSS fix data.
 */
void handleGnssFix(const GnssFix *gnssOutput)
{
    logInfoFormatted("%02u:%02u:%02u,%.06f,%.06f,%.06f", gnssOutput->datetime.hour,
                    gnssOutput->datetime.minute, gnssOutput->datetime.seconds,
                    gnssOutput->latitude, gnssOutput->longitude, gnssOutput->accuracy);

    int ret = modem_info_params_get(&modem_param_info);
    
    if (0 == ret)
    {
        int32_t Data[MAX_TRACKS_COUNT] = {0};
        Data[CELL_ID_TRACK_ID] = (int32_t) modem_param_info.network.cellid_dec;
        addColumnToTape(I_TAPE, FIX_INTERVAL_S, Data);
    }

    // Force send the GNSS data to the cloud
    triggerUpload();
}

/**
 * @brief Application setup/init code
 *
 * This function sets up the system, defines the GNSS settings and starts GNSS acquisition.
 */
void appInit(void)
{
    GnssSettings gnss_settings =
    {
        .fix_interval       = FIX_INTERVAL_S,
        .fix_retry          = FIX_RETRY_S,
        .dynamics_mode      = GENERAL_PURPOSE,
        .callback = handleGnssFix,
    };
    startGnss(&gnss_settings);

    modem_info_params_init(&modem_param_info);
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
 *   void onEventName(const void *p_data, int i_data)
 *
 * Unimplemented callbacks are ignored by the SDK.
 */
void onDeviceInitOk(const void *p_data, int i_data)
{
    logInfo("All system parts initialized successfully");
}


void onTerminalCmd(const void *p_data, int i_data)
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
