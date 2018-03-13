#include "slog/slog.h"
#include "slog_threads.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * Internal API prototypes
 */
static char*        slog_level_to_string(enum slog_level level);
static struct slog* slog_construct(FILE*                 handle,
                                   uint32_t              closable,
                                   enum slog_buffer_size buffer_size);

struct slog
{
	FILE*    handle;
	uint32_t closeable_handle;

	enum slog_level max_level;

	char* prefix;

	char*  buffer;
	size_t buffer_size;

	struct slog_mutex* slog_mutex;
};

SLOG_API struct slog* slog_open_log_file(char const*           filename,
                                         enum slog_buffer_size buffer_size)
{
	FILE* handle;
	int   error = fopen_s(&handle, filename, "w");
	if (error != 0)
	{
		fprintf_s(stderr,
		          "cannot open file '%s': %s\n",
		          filename,
		          strerror(error));
	}

	return slog_construct(handle, 1, buffer_size);
}

SLOG_API struct slog* slog_open_log_console(FILE* console_handle,
                                            enum slog_buffer_size buffer_size)
{
	return slog_construct(console_handle, 1, buffer_size);
}

SLOG_API void slog_set_prefix(struct slog* const slog,
                              char const*        prefix,
                              size_t const       length)
{
	if (slog->prefix)
	{
		free(slog->prefix);
	}

	slog->prefix = malloc(sizeof(char) * length);
	memset(slog->prefix, 0, sizeof(sizeof(char) * length));

	strcpy_s(slog->prefix, length, prefix);
}

SLOG_API void slog_set_max_loglevel(struct slog* const    slog,
                                    enum slog_level const level)
{
	slog->max_level = level;
}

SLOG_API int slog_log(struct slog*          slog,
                      enum slog_level const level,
                      char const*           format,
                      ...)
{
	char buffer[255] = {'\0'};

	va_list list_pointer;
	va_start(list_pointer, format);
	vsnprintf(buffer, sizeof(buffer) - 1, format, list_pointer);
	va_end(list_pointer);

	slog_threads_lock_mutex(slog->slog_mutex);
	fprintf(slog->handle,
	        "%s %s %s",
	        slog_level_to_string(level),
	        slog->prefix,
	        buffer);

	fflush(slog->handle);
	slog_threads_release_mutex(slog->slog_mutex);
	return 0;
}

SLOG_API void slog_close(struct slog* slog)
{
	if (slog->closeable_handle && slog->handle)
	{
		int result = fclose(slog->handle);

		if (result != 0)
		{
			fprintf(stderr,
			        "%s Failed to close log file.\n",
			        slog_level_to_string(SLOG_LEVEL_ERROR));
		}
	}

	slog_threads_destroy_mutex(slog->slog_mutex);

	free(slog);
}

/**
 * SLog Internal API
 */
static struct slog* slog_construct(FILE*                 handle,
                                   uint32_t              closable,
                                   enum slog_buffer_size buffer_size)
{
	struct slog* slog = malloc(sizeof(struct slog));
	memset(slog, 0, sizeof(struct slog));

	slog->handle           = handle;
	slog->closeable_handle = closable;

	slog->slog_mutex = slog_threads_create_mutex();

	return slog;
}

static char* slog_level_to_string(enum slog_level level)
{
	switch (level)
	{
	default: return "";
	case SLOG_LEVEL_DEBUG: return "[ DEBUG   ]";
	case SLOG_LEVEL_INFO: return "[ INFO    ]";
	case SLOG_LEVEL_WARN: return "[ WARNING ]";
	case SLOG_LEVEL_ERROR: return "[ ERROR   ]";
	case SLOG_LEVEL_FATAL: return "[ FATAL   ]";
	}
}
