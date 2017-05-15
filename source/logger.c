#include "logger.h"
#include "logger_threads.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * Internal API prototypes
 */
static char* logger_level_to_string(enum logger_level level);
static struct logger* logger_construct(FILE*                   handle,
                                       uint32_t                closable,
                                       enum logger_buffer_size buffer_size);

struct logger
{
	FILE*    handle;
	uint32_t closeable_handle;

	enum logger_level max_level;

	char* prefix;

	char*  buffer;
	size_t buffer_size;

	struct logger_mutex* logger_mutex;
};

SLOG_API struct logger* logger_open_log_file(char const*             filename,
                                             enum logger_buffer_size buffer_size)
{
	FILE* handle;
	int   error = fopen_s(&handle, filename, "w");
	if (error != 0)
	{
		fprintf_s(stderr, "cannot open file '%s': %s\n", filename, strerror(error));
	}

	return logger_construct(handle, 1, buffer_size);
}

SLOG_API struct logger* logger_open_log_console(FILE*                   console_handle,
                                                enum logger_buffer_size buffer_size)
{
	return logger_construct(console_handle, 1, buffer_size);
}

SLOG_API void logger_set_prefix(struct logger* const logger,
                                char const*          prefix,
                                size_t const         length)
{
	if (logger->prefix)
	{
		free(logger->prefix);
	}

	logger->prefix = malloc(sizeof(char) * length);
	memset(logger->prefix, 0, sizeof(sizeof(char) * length));

	strcpy_s(logger->prefix, length, prefix);
}

SLOG_API void logger_set_max_loglevel(struct logger* const logger, enum logger_level const level)
{
	logger->max_level = level;
}

SLOG_API int logger_log(struct logger*          logger,
                        enum logger_level const level,
                        char const*             format,
                        ...)
{
	char buffer[255] = {'\0'};

	va_list list_pointer;
	va_start(list_pointer, format);
	vsnprintf(buffer, sizeof(buffer) - 1, format, list_pointer);
	va_end(list_pointer);

	logger_threads_lock_mutex(logger->logger_mutex);
	fprintf(logger->handle, "%s %s %s", logger_level_to_string(level), logger->prefix, buffer);

	fflush(logger->handle);
	logger_threads_release_mutex(logger->logger_mutex);
	return 0;
}

SLOG_API void logger_close(struct logger* logger)
{
	if (logger->closeable_handle && logger->handle)
	{
		int result = fclose(logger->handle);

		if (result != 0)
		{
			fprintf(stderr,
			        "%s Failed to close log file.\n",
			        logger_level_to_string(LOGGER_LEVEL_ERROR));
		}
	}

	logger_threads_destroy_mutex(logger->logger_mutex);

	free(logger);
}

/**
 * SLog Internal API
 */
static struct logger* logger_construct(FILE*                   handle,
                                       uint32_t                closable,
                                       enum logger_buffer_size buffer_size)
{
	struct logger* logger = malloc(sizeof(struct logger));
	memset(logger, 0, sizeof(struct logger));

	logger->handle           = handle;
	logger->closeable_handle = closable;

	logger->logger_mutex = logger_threads_create_mutex();

	return logger;
}

static char* logger_level_to_string(enum logger_level level)
{
	switch (level)
	{
	default: return "";
	case LOGGER_LEVEL_DEBUG: return "[ DEBUG   ]";
	case LOGGER_LEVEL_INFO: return "[ INFO    ]";
	case LOGGER_LEVEL_WARN: return "[ WARNING ]";
	case LOGGER_LEVEL_ERROR: return "[ ERROR   ]";
	case LOGGER_LEVEL_FATAL: return "[ FATAL   ]";
	}
}
