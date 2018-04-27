#include "slog_file.h"

#include <assert.h>
#include <errno.h>

int fopen_s(FILE** handle, const char *filename, const char *mode)
{
	int error = 0;

	assert(handle);

	*handle = fopen(filename, mode);
	if (!handle)
	{
		error = errno;
	}

	return error;
}
