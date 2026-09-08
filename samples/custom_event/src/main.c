#include "lmt_sdk_app.h"
#include "date_time.h"

// Feel free to edit the following file content

#define MAIN_BOOT_OK_BIT FIRST_USER_STATUS_BIT  // This bit in this app indicates that app is up and running
#define MAIN_BOOT_OK_VAL BIT(MAIN_BOOT_OK_BIT)  // Define the corresponding bit mask value

#define EVENT_TIMEOUT K_MINUTES(1)

#define EVENT_STRING_MAX_SIZE 32


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
 * @brief Function sends custom event
 * 
 * Sample function to send a custom event.
 * Creating multiple custom events based on repeated calls.
 */
void generateEvent(void)
{
    char event_name[EVENT_STRING_MAX_SIZE] = {0};
    char event_value[EVENT_STRING_MAX_SIZE] = {0};
    uint64_t event_timestamp = 0;
    static int event_counter = 0;
    EventSeverity severity = EventSeverity_NOT_SPECIFIED;

    switch (event_counter)
    {
        case 0:
            strcpy(event_name, "TEST_RESULT");
            strcpy(event_value, "FAILED");
            date_time_now(&event_timestamp);
            severity = EventSeverity_CRITICAL;
            break;

        case 1:
            strcpy(event_name, "TEST_RESULT");
            strcpy(event_value, "PASS");
            event_timestamp = 0;
            severity = EventSeverity_WARNING;
            break;

        case 2:
            strcpy(event_name, "TEST_RESULT");
            strcpy(event_value, "PASS");
            date_time_now(&event_timestamp);
            severity = EventSeverity_INFO;
            break;

        case 3:
            strcpy(event_name, "HEARTBEAT");
            strcpy(event_value, "OK");
            date_time_now(&event_timestamp);
            break;
    }

    sendCustomEvent(event_name, event_value, event_timestamp, severity);

    event_counter = (event_counter + 1) % 4;
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
        k_sleep(EVENT_TIMEOUT);
        generateEvent();
    }

    return 0;
}
