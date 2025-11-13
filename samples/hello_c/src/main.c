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
 * @brief Event callback function.
 *        Declared in SDK. Can be not defined if user does not want to handle SoM events
 */
void somEventHandler(SomEvent event, void *data, uint8_t data_len)
{
    switch(event)
    {
        case EVENT_DEVICE_INIT_OK:
            logInfo("Hello world!");
            break;

        case EVENT_TERMINAL_CMD:
            // Perform the requested command and send the result
            sendEventCmdRes(runTerminalCmd(data, data_len));
            break;

        default:
            break;
    }
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
