#ifndef _SLOG_THREADS_H_
#define _SLOG_THREADS_H_

struct slog_mutex;

struct slog_mutex* slog_threads_create_mutex(void);

void slog_threads_destroy_mutex(struct slog_mutex* mutex);

void slog_threads_lock_mutex(struct slog_mutex* mutex);

void slog_threads_release_mutex(struct slog_mutex* mutex);

#endif // _SLOG_THREADS_H_
