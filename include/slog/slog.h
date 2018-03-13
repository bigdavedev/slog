#ifndef _SLOG_H_
#define _SLOG_H_

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

struct slog;

enum slog_level
{
	SLOG_LEVEL_DEBUG,
	SLOG_LEVEL_INFO,
	SLOG_LEVEL_WARN,
	SLOG_LEVEL_ERROR,
	SLOG_LEVEL_FATAL
};

enum slog_buffer_size
{
	SLOG_BUFFER_NOTHING = 0,
	SLOG_BUFFER_2KiB    = 2048,
	SLOG_BUFFER_16KiB   = 16384,
	SLOG_BUFFER_32KiB   = 32768,
	SLOG_BUFFER_64Kib   = 65536,

	SLOG_BUFFER_DEFAULT = SLOG_BUFFER_NOTHING
};

SLOG_API struct slog* slog_open_log_file(char const*           filename,
                                         enum slog_buffer_size buffer_size);

SLOG_API struct slog* slog_open_log_console(FILE* console_handle,
                                            enum slog_buffer_size buffer_size);

SLOG_API void slog_set_max_loglevel(struct slog* const    slog,
                                    enum slog_level const level);

SLOG_API void slog_set_prefix(struct slog* const slog,
                              char const*        prefix,
                              size_t const       length);

SLOG_API int slog_log(struct slog*          slog,
                      enum slog_level const level,
                      char const*           format,
                      ...);

SLOG_API void slog_close(struct slog* slog);

#endif // _SLOG_H_
