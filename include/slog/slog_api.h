#ifndef _SLOG_API_H_
#define _SLOG_API_H_

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

#endif
