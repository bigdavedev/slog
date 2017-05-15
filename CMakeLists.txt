cmake_minimum_required (VERSION 3.5)

option (SLOG_BUILD_SHARED_LIBRARY ON)

# Simple Logger
project (slog C)

set (TARGET ${PROJECT_NAME} CACHE INTERNAL "")

if (SLOG_BUILD_SHARED_LIBRARY)
	add_library (${TARGET} SHARED "")
else ()
	add_library (${TARGET} "")
endif ()

if (UNIX AND NOT APPLE)
	set(THREADS_PREFER_PTHREAD_FLAG ON CACHE INTERNAL "")
	find_package (Threads REQUIRED)
endif ()

target_compile_definitions (
	${TARGET}

	PUBLIC
	__STDC_WANT_LIB_EXT1__=1
)

target_compile_options (
	${TARGET}

	PUBLIC
	$<$<C_COMPILER_ID:GNU>:-std=c11>
	$<$<C_COMPILER_ID:Clang>:-std=c11>
)

target_include_directories (
	${TARGET}
	
	PUBLIC
	${CMAKE_SOURCE_DIR}/include
)

target_sources (
	${TARGET}

	PRIVATE
	${CMAKE_SOURCE_DIR}/source/logger.c
	$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/source/logger_threads_windows.c>
	$<$<PLATFORM_ID:Linux>:${CMAKE_SOURCE_DIR}/source/logger_threads_unix.c>
	
	PRIVATE
	${CMAKE_SOURCE_DIR}/include/logger.h
	${CMAKE_SOURCE_DIR}/include/logger_threads.h
)

target_link_libraries (
	${TARGET}

	PUBLIC
	$<$<PLATFORM_ID:Linux>:Threads::Threads>
)
