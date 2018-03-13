#ifndef _SLOG_H_
#define _SLOG_H_

#include "slog/slog_api.h"

#include <stdio.h>

struct slog;

/**
 * Log levels supported by Slog
 */
enum slog_level
{
	SLOG_LEVEL_DEBUG,
	SLOG_LEVEL_INFO,
	SLOG_LEVEL_WARN,
	SLOG_LEVEL_ERROR,
	SLOG_LEVEL_FATAL
};

/**
 * In order to support log rotation, use the following levels
 */
enum slog_buffer_size
{
	SLOG_BUFFER_NOTHING,
	SLOG_BUFFER_2KiB,
	SLOG_BUFFER_16KiB,
	SLOG_BUFFER_32KiB,
	SLOG_BUFFER_64Kib,

	SLOG_BUFFER_DEFAULT = SLOG_BUFFER_NOTHING
};

/**
 * Create a file to log to
 */
SLOG_API struct slog* slog_open_log_file(char const*           filename,
                                         enum slog_buffer_size buffer_size);

/**
 * Use the console for logging
 */
SLOG_API struct slog* slog_open_log_console(FILE* console_handle,
                                            enum slog_buffer_size buffer_size);

/**
 * Cap the warning level
 */
SLOG_API void slog_set_max_loglevel(struct slog* const    slog,
                                    enum slog_level const level);

/**
 * Set an optional prefix to the log messages
 */
SLOG_API void slog_set_prefix(struct slog* const slog,
                              char const*        prefix,
                              size_t const       length);

/**
 * Log some information to the log
 */
SLOG_API int slog_log(struct slog*          slog,
                      enum slog_level const level,
                      char const*           format,
                      ...);

/**
 * Close the log when finished with it
 */
SLOG_API void slog_close(struct slog* slog);

#endif // _SLOG_H_
