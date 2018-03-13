#include "slog_threads.h"

#include <pthread.h>

#include <memory.h>
#include <stdlib.h>

struct slog_mutex
{
	pthread_mutex_t mutex;
};

struct slog_mutex* slog_threads_create_mutex(void)
{
	struct slog_mutex* mutex = malloc(sizeof(struct slog_mutex));
	memset(mutex, 0, sizeof(struct slog_mutex));

	int result = pthread_mutex_init(&mutex->mutex, NULL);

	return mutex;
}

void slog_threads_destroy_mutex(struct slog_mutex* mutex)
{
	int result = pthread_mutex_trylock(&mutex->mutex);

	if (result == 0)
	{
		pthread_mutex_destroy(&mutex->mutex);
	}
}

void slog_threads_lock_mutex(struct slog_mutex* mutex)
{
	pthread_mutex_lock(&mutex->mutex);
}

void slog_threads_release_mutex(struct slog_mutex* mutex)
{
	pthread_mutex_unlock(&mutex->mutex);
}
