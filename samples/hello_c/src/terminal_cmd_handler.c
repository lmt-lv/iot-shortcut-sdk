#include "terminal_cmd_handler.h"
#include "lmt_coap_manager.h"
#include "lmt_storage_manager.h"
#include <errno.h>
#include <string.h>
#include <zephyr/sys/reboot.h>

// List of supported commands
#define CMD_ERASE_FLASH 'E'
#define CMD_LOG_STATUS  'L'
#define CMD_REBOOT      'R'

#define PRE_REBOOT_DELAY K_SECONDS(10)

int runTerminalCmd(uint8_t *cmd, uint8_t cmd_len)
{
    int err = 0;

    if(cmd == NULL || cmd_len == 0)
    {
        return -EINVAL;
    }

    switch(*cmd)
    {
        case CMD_ERASE_FLASH:
            err = eraseFlash(); // get rid of log files
            if(err)
            {
                return err;
            }
            break;

        case CMD_LOG_STATUS:
            logInfo("Log entry created via terminal command");
            break;

        case CMD_REBOOT:
            k_sleep(PRE_REBOOT_DELAY);
            modemShutdown();
            sys_reboot(SYS_REBOOT_COLD);
            break;

        default:
            return -ENOTSUP;
    }

    return 0;
}
