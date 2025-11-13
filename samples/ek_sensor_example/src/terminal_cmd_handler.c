
/*
 * terminal_cmd_handler.c
 *
 * This file implements a simple command handler for terminal/serial commands.
 *
 * Currently, the handler is designed to process single-character commands (e.g., 'E' for erase, 'R'
 * for reboot). The implementation can be easily extended to support multi-character or more complex
 * commands in the future.
 */

#include "terminal_cmd_handler.h"
#include "lmt_coap_manager.h"
#include "lmt_storage_manager.h"
#include <errno.h>
#include <string.h>
#include <zephyr/sys/reboot.h>

// List of supported commands
#define CMD_ERASE_FLASH 'E'
#define CMD_REBOOT      'R'

#define PRE_REBOOT_DELAY K_SECONDS(2)

/**
 * @brief Handle a terminal/serial command.
 *
 * This function processes a command received as a byte array. Currently, only the first character
 * is used. Supported commands:
 *   - 'E': Erase flash (delete log files)
 *   - 'R': Reboot the system
 *
 * The handler can be easily extended to support more or multi-character commands in the future.
 *
 * @param cmd Pointer to the command data (array of bytes)
 * @param cmd_len Number of bytes in the command
 * @return 0 on success, negative error code on failure or unsupported command
 */
int runTerminalCmd(uint8_t *cmd, uint8_t cmd_len)
{
    int error = 0;

    // Check for valid input
    if(cmd == NULL || cmd_len == 0)
    {
        return -EINVAL; // Invalid argument
    }

    // Only the first character of the command is used
    switch(*cmd)
    {
        case CMD_ERASE_FLASH:
            // Erase flash memory (e.g., delete log files)
            error = eraseFlash();
            if(error)
            {
                return error; // Return error code if erase fails
            }
            break;

        case CMD_REBOOT:
            // Shut down modem, log a warning, wait, then reboot the system
            modemShutdown();
            logWarning("Rebooting system...");
            k_sleep(PRE_REBOOT_DELAY);   // Wait before rebooting
            sys_reboot(SYS_REBOOT_COLD); // Perform a cold reboot
            break;

        default:
            // Command not supported
            return -ENOTSUP;
    }

    // Command handled successfully
    return 0;
}
