
/**
 * @file terminal_cmd_handler.h
 * @brief Interface for running terminal commands from code
 *
 * This header provides a function to run a command received as a sequence of bytes (received from
 * server).
 */

#ifndef TERMINAL_CMD_HANDLER_H
#define TERMINAL_CMD_HANDLER_H

#include <stdint.h>

/**
 * @brief Run a command received as a sequence of bytes.
 *
 * This function takes a command (as an array of bytes) and its length, and executes it.
 *
 * @param cmd Pointer to the command data (array of bytes)
 * @param cmd_len Number of bytes in the command
 * @return 0 on success, negative error code on failure
 */
int runTerminalCmd(uint8_t *cmd, uint8_t cmd_len);

#endif // TERMINAL_CMD_HANDLER_H
