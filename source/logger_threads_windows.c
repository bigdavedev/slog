#include "logger_threads.h"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

#include <stdlib.h>

struct logger_mutex
{
	CRITICAL_SECTION critical_section;
};

struct logger_mutex* logger_threads_create_mutex(void)
{
	struct logger_mutex* mutex = malloc(sizeof(struct logger_mutex));
	memset(mutex, 0, sizeof(struct logger_mutex));

	InitializeCriticalSectionAndSpinCount(&mutex->critical_section, 0x00000400);

	return mutex;
}

void logger_threads_destroy_mutex(struct logger_mutex* mutex)
{
	DeleteCriticalSection(&mutex->critical_section);

	free(mutex);
}

void logger_threads_lock_mutex(struct logger_mutex* mutex)
{
	EnterCriticalSection(&mutex->critical_section);
}

void logger_threads_release_mutex(struct logger_mutex* mutex)
{
	LeaveCriticalSection(&mutex->critical_section);
}
