#include "logger_threads.h"

#include <pthread.h>

#include <memory.h>
#include <stdlib.h>

struct logger_mutex
{
	pthread_mutex_t mutex;
};

struct logger_mutex* logger_threads_create_mutex(void)
{
	struct logger_mutex* mutex = malloc(sizeof(struct logger_mutex));
	memset(mutex, 0, sizeof(struct logger_mutex));

	int result = pthread_mutex_init(&mutex->mutex, NULL);

	return mutex;
}

void logger_threads_destroy_mutex(struct logger_mutex* mutex)
{
	int result = pthread_mutex_trylock(&mutex->mutex);

	if (result == 0)
	{
		pthread_mutex_destroy(&mutex->mutex);
	}
}

void logger_threads_lock_mutex(struct logger_mutex* mutex)
{
	pthread_mutex_lock(&mutex->mutex);
}

void logger_threads_release_mutex(struct logger_mutex* mutex)
{
	pthread_mutex_unlock(&mutex->mutex);
}
