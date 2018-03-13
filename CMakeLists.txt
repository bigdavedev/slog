cmake_minimum_required (VERSION 3.5)

# Simple Logger
project (slog VERSION 0.1.0 LANGUAGES C)

add_library (slog source/logger.c)

if (UNIX AND NOT APPLE)
	set(THREADS_PREFER_PTHREAD_FLAG ON CACHE INTERNAL "")
	find_package (Threads REQUIRED)
endif ()

target_compile_definitions (
	${PROJECT_NAME}

	PRIVATE
	__STDC_WANT_LIB_EXT1__=1
)

target_compile_features (
	${PROJECT_NAME}

	PUBLIC
	$<BUILD_INTERFACE:c_variadic_macros>
	$<INSTALL_INTERFACE:c_variadic_macros>
)

set_target_properties (
	${PROJECT_NAME}
	PROPERTIES
		C_EXTENSIONS OFF
)

target_include_directories (
	${PROJECT_NAME}
	
	PUBLIC
	${CMAKE_SOURCE_DIR}/include
)

target_sources (
	${PROJECT_NAME}

	PRIVATE
	$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/source/logger_threads_windows.c>
	$<$<PLATFORM_ID:Linux>:${CMAKE_SOURCE_DIR}/source/logger_threads_unix.c>
	
	PRIVATE
	${CMAKE_SOURCE_DIR}/include/logger.h
	${CMAKE_SOURCE_DIR}/include/logger_threads.h
)

target_link_libraries (
	${PROJECT_NAME}

	PRIVATE
	$<$<PLATFORM_ID:Linux>:Threads::Threads>
)
