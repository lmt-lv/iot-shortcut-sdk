#include <zephyr/kernel.h>
#include "lmt_sdk_api.h"

#if defined(CONFIG_COAP_QUEUE_SIZE)
#define MAX_COUNT_OF_COAP_REQUESTS CONFIG_COAP_QUEUE_SIZE
#else
#define MAX_COUNT_OF_COAP_REQUESTS 96
#endif

// Substitutes
uint8_t coap_queue[MAX_COUNT_OF_COAP_REQUESTS * APP_COAP_MAX_MSG_LEN];
const unsigned int coap_queue_size = MAX_COUNT_OF_COAP_REQUESTS * APP_COAP_MAX_MSG_LEN;

void setup(void)
{
    logInfo("Setup function called");
}

void loop(void)
{
    logWarning("Loop function called");
    k_sleep(K_FOREVER);
}

int main(void)
{
    enableSerialLog();
    lmtInit();
    setup();
    logInfo("Hello World!");
    // disableSerialLog();
    logInfo("Serial log should be disabled, this won't be printed");
    while(1)
    {
        loop();
    }
    return 0;
}
