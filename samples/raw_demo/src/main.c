/**
 * @file main.c
 * @brief Main application file for LMT SDK sample
 *
 * This file contains a sample for sending raw (unformatted) uplink data with the IoT Shortcut
 * SDK. GNSS is started as well, so fixes are acquired alongside the raw uplinks.
 *
 */
#include "lmt_sdk_app.h"

#define CELL_ID_TRACK_ID 0
#define FIX_INTERVAL_S 60
#define FIX_RETRY_S 45

#define MAILER_LOOP_TIME K_MINUTES(3)

#define UL_MODE_NORMAL 0
#define UL_MODE_RAW 1

/* Encoding that a raw payload is meant to represent. */
enum raw_type
{
    RAW_HEX = 0,
    RAW_UTF8,
    RAW_ASCII,
    RAW_BASE64,
    RAW_BIN
};

/* One raw uplink payload: what it is, where it lives and how long it is. */
struct raw_payload
{
    enum raw_type type;
    const char *name;
    uint8_t *data;
    uint16_t len;
};

/* The SDK keeps the pointer handed to setRawData(), so the buffers must outlive the call. */
static uint8_t raw_hex_data[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0xCA, 0xFE, 0xBA, 0xBE};
static uint8_t raw_utf8_data[] = {"Hello, IoT"};

/* For buffers initialized as a byte list. */
#define RAW_PAYLOAD(raw_fmt, buf) \
    { .type = (raw_fmt), .name = #raw_fmt, .data = (buf), .len = sizeof(buf) }

/* For buffers initialized from a string literal - drops the terminating NUL from the payload. */
#define RAW_PAYLOAD_STR(raw_fmt, buf) \
    { .type = (raw_fmt), .name = #raw_fmt, .data = (buf), .len = sizeof(buf) - 1 }

/* Sent one per mailer loop. Add an entry here to send another format. */
static const struct raw_payload raw_payloads[] =
{
    RAW_PAYLOAD(RAW_HEX, raw_hex_data),
    RAW_PAYLOAD_STR(RAW_UTF8, raw_utf8_data),
};

/**
 * @brief GNSS fix handler.
 *
 * Is called when a GNSS fix is acquired. Data from the fix is printed to the log. 
 * 
 * @param[in] gnssOutput Pointer to a structure containing acquired GNSS fix data.
 */
void handleGnssFix(const GnssFix *gnssOutput)
{
    logInfoFormatted("%02u:%02u:%02u, %.06f, %.06f, %.06f", gnssOutput->datetime.hour,
                    gnssOutput->datetime.minute, gnssOutput->datetime.seconds,
                    gnssOutput->latitude, gnssOutput->longitude, (double)gnssOutput->accuracy);
}

/**
 * @brief Send the next payload from the table.
 *
 * In raw mode the SDK uploads the buffer as-is, without any formatting, so the server receives
 * exactly the bytes listed in the payload table. Passing true as the upload flag makes the mailer
 * send it right away instead of waiting for the next uplink period.
 */
static void sendNextRawPayload(void)
{
    static size_t next = 0;

    const struct raw_payload *payload = &raw_payloads[next];
    int ret = setRawData(payload->data, payload->len, true);

    if (0 == ret)
    {
        logInfoFormatted("Sent %s payload, %u bytes", payload->name, payload->len);
    }
    else
    {
        logError("Failed to set raw data", ret);
    }

    next = (next + 1) % ARRAY_SIZE(raw_payloads);
}

/**
 * @brief Application setup/init code
 *
 * This function sets up the system, defines the GNSS settings, starts GNSS acquisition and
 * switches the uplink to raw mode so that setRawData() payloads are sent unformatted.
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
    setUlDataMode(UL_MODE_RAW);
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
    // Initialize the LMT SDK (sets up core system functions)
    lmtInit();
    // Initialize the user application.
    appInit();   

    while(1)
    {
        k_sleep(MAILER_LOOP_TIME);
        sendNextRawPayload();
    }

    return 0;
}
