#ifndef LMT_FILESYSTEM_H
#define LMT_FILESYSTEM_H

#include <stdbool.h>
#include <stddef.h>
#include <zephyr/storage/flash_map.h>

/**
 * @brief Creates and initializes LittleFS file system.
 *
 * @return 0 on success, negative errno code on fail.
 */
int fsInit(void);

/**
 * @brief Unmounts and deinitializes LittleFS file system.
 *
 * @return 0 on success, negative errno code on fail.
 */
int fsDeinit(void);

/**
 * @brief Erase external flash memory.
 *
 * @return 0 on success, negative errno code on fail.
 */
int flashErase(void);

/**
 * @brief Writes text to a log file.
 *
 * @param filename The name of the file to write to.
 * @param text The text to write to the file.
 * @return The number of bytes written on success, negative value otherwise
 */
int fileWrite(const char *filename, char *text);

/**
 * @brief Creates or overwrites file
 *
 * @param filename The name of the file to write to.
 * @param text The text to write to the file.
 * @return The number of bytes written on success, negative value otherwise
 */
int fileOverwrite(const char *filename, char *text);

/**
 * @brief Reads text from a log file.
 *
 * @param filename The name of the file to read from.
 * @param read_buffer The buffer to store the read text.
 * @param buffer_size The size of the buffer.
 * @param offset The offset to start reading from.
 * @param file_size The size of the file.
 * @return The number of bytes written on success, negative value otherwise
 */
int fileRead(const char *filename, char *read_buffer, size_t buffer_size, size_t offset,
             size_t file_size);

/**
 * @brief Gets the size of a file.
 *
 * @param filename The name of the file to read from.
 * @return The size of the file in bytes on success, negative value otherwise
 */
int getFileSize(const char *filename);

/**
 * @brief Sets the amount of free space on the volume.
 *
 * @param free_space The pointer to store the free space in.
 * @return 0 on success, -1 on fail.
 */
int getVolumeFreeSpace(unsigned long *free_space);

/**
 * @brief Finds the oldest file with a given prefix by comparing timestamps.
 *
 * @param fileprefix The prefix of the file to search for - "app_" usually
 * @param oldest_file argument where the oldest file name will be stored.
 * @param upload true when all files with the same prefix need to be uploaded
 * @return number of files found with given prefix.
 */
char getOldestFile(const char *fileprefix, char *oldest_file, bool upload);

/**
 * @brief Deletes file from the file system.
 *
 * @param filename The name of the file to delete.
 * @return 0 on success, negative errno code on fail
 */
int deleteFile(const char *filename);

/**
 * @brief Function to rename file
 *
 * @param old_name The old name of the file
 * @param new_name The new name of the file
 * @return 0 on success, negative errno code on fail
 */
int renameFile(const char *old_name, const char *new_name);

#endif // LMT_FILESYSTEM_H