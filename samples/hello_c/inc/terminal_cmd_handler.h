#ifndef TERMINAL_CMD_HANDLER_H
#define TERMINAL_CMD_HANDLER_H

#include <stdint.h>

/**
 * @brief Runs the command
 *
 * @return 0 on success, negative error code on fail
 */
int runTerminalCmd(uint8_t *cmd, uint8_t cmd_len);

#endif // TERMINAL_CMD_HANDLER_H
