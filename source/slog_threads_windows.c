#include "slog_threads.h"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

#include <stdlib.h>

struct slog_mutex
{
	CRITICAL_SECTION critical_section;
};

struct slog_mutex* slog_threads_create_mutex(void)
{
	struct slog_mutex* mutex = malloc(sizeof(struct slog_mutex));
	memset(mutex, 0, sizeof(struct slog_mutex));

	InitializeCriticalSectionAndSpinCount(&mutex->critical_section, 0x00000400);

	return mutex;
}

void slog_threads_destroy_mutex(struct slog_mutex* mutex)
{
	DeleteCriticalSection(&mutex->critical_section);

	free(mutex);
}

void slog_threads_lock_mutex(struct slog_mutex* mutex)
{
	EnterCriticalSection(&mutex->critical_section);
}

void slog_threads_release_mutex(struct slog_mutex* mutex)
{
	LeaveCriticalSection(&mutex->critical_section);
}
