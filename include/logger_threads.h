#ifndef _LOGGER_THREADS_H_
#define _LOGGER_THREADS_H_

struct logger_mutex;

struct logger_mutex* logger_threads_create_mutex(void);

void logger_threads_destroy_mutex(struct logger_mutex* mutex);

void logger_threads_lock_mutex(struct logger_mutex* mutex);

void logger_threads_release_mutex(struct logger_mutex* mutex);

#endif // _LOGGER_THREADS_H_
