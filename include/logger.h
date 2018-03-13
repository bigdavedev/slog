#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>

#ifdef SLOG_STATIC_DEFINE
#  define SLOG_API
#  define SLOG_NO_EXPORT
#else
#  ifndef SLOG_API
#    ifdef slog_EXPORTS
#      ifdef WIN32
#        define SLOG_API __declspec(dllexport)
#      else
#        define SLOG_API __attribute__((visibility("default")))
#      endif
#    else
#      ifdef WIN32
#        define SLOG_API __declspec(import)
#      else
#        define SLOG_API __attribute__((visibility("default")))
#      endif
#    endif
#  endif
#endif

struct logger;

enum logger_level
{
	LOGGER_LEVEL_DEBUG,
	LOGGER_LEVEL_INFO,
	LOGGER_LEVEL_WARN,
	LOGGER_LEVEL_ERROR,
	LOGGER_LEVEL_FATAL
};

enum logger_buffer_size
{
	LOGGER_BUFFER_NOTHING = 0,
	LOGGER_BUFFER_2KiB    = 2048,
	LOGGER_BUFFER_16KiB   = 16384,
	LOGGER_BUFFER_32KiB   = 32768,
	LOGGER_BUFFER_64Kib   = 65536,

	LOGGER_BUFFER_DEFAULT = LOGGER_BUFFER_NOTHING
};

SLOG_API struct logger* logger_open_log_file(char const*             filename,
                                             enum logger_buffer_size buffer_size);

SLOG_API struct logger* logger_open_log_console(FILE*                   console_handle,
                                                enum logger_buffer_size buffer_size);

SLOG_API void logger_set_max_loglevel(struct logger* const logger, enum logger_level const level);

SLOG_API void logger_set_prefix(struct logger* const logger,
                                char const*          prefix,
                                size_t const         length);

SLOG_API int logger_log(struct logger*          logger,
                        enum logger_level const level,
                        char const*             format,
                        ...);

SLOG_API void logger_close(struct logger* logger);

#endif // _LOGGER_H_
