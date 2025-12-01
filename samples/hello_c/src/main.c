#include "lmt_sdk_app.h"

// Do not delete the following function in order to handle the linkers error
void force_link_k_msgq_put(struct k_msgq *q, void *data, k_timeout_t t)
{
    k_msgq_put(q, data, t);
}

// Feel free to edit the following file content

#define MAIN_BOOT_OK_BIT FIRST_USER_STATUS_BIT  // This bit in this app indicates that app is up and running
#define MAIN_BOOT_OK_VAL BIT(MAIN_BOOT_OK_BIT)  // Define the corresponding bit mask value

#define MAILER_LOOP_TIME K_MINUTES(3)

/**
 * @brief Application setup/init code
 */
void usrInit(void)
{
    // Set app boot ok mask: when all bits in mask are set,
    // the EVENT_DEVICE_INIT_OK for somEventHandler will be generated
    setUserBootOkMask(MAIN_BOOT_OK_VAL);

    // Leave a note in log that system's up and running
    setBootOkBit(MAIN_BOOT_OK_BIT);
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
    logInfo("Hello world!");
}

void onTerminalCmd(void *p_data, int i_data)
{
    // Perform the requested command and send the result
    sendEventCmdRes(runTerminalCmd(p_data, i_data));
}

/**
 * @brief Application entry
 *
 * @return Returns error code that should be returned never
 */
int main(void)
{
    // The init order depends on the Setup called functions
    lmtInit();  // init the lmtSDK first if Setup uses lmtSDK functionality
    usrInit();  // init the application

    while(1)
    {
        k_sleep(MAILER_LOOP_TIME);
        triggerMailer(true);
    }

    return 0;
}
