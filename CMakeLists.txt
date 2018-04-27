cmake_minimum_required (VERSION 3.5)

# Simple Logger
project (slog VERSION 0.1.0 LANGUAGES C)

add_library (slog source/slog.c)
add_library (Slog::Slog ALIAS slog)

set_target_properties (
	slog
	PROPERTIES
		C_EXTENSIONS OFF
		C_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN 1
)

if (UNIX AND NOT APPLE)
	set(THREADS_PREFER_PTHREAD_FLAG ON CACHE INTERNAL "")
	find_package (Threads REQUIRED)
endif ()

target_compile_definitions (
	slog

	PRIVATE
	__STDC_WANT_LIB_EXT1__=1
)

target_compile_features (
	slog

	PUBLIC
	$<BUILD_INTERFACE:c_static_assert>
	$<INSTALL_INTERFACE:c_variadic_macros>
)

target_include_directories (
	slog
	
	PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

target_sources (
	slog

	PRIVATE
	$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/source/slog_threads_windows.c>
	$<$<PLATFORM_ID:Linux>:
		${CMAKE_SOURCE_DIR}/source/slog_threads_unix.c
		${CMAKE_SOURCE_DIR}/source/slog_file_unix.c>
	
	PRIVATE
	${CMAKE_SOURCE_DIR}/include/slog/slog.h
	${CMAKE_SOURCE_DIR}/include/slog/slog_api.h
	${CMAKE_SOURCE_DIR}/include/slog_file.h
	${CMAKE_SOURCE_DIR}/include/slog_threads.h
)

target_link_libraries (
	slog

	PRIVATE
	$<$<PLATFORM_ID:Linux>:Threads::Threads>
)

## Setup installation
include (GNUInstallDirs)
set (INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Slog)

install (
	TARGETS
		slog
	EXPORT
		slog-targets
	LIBRARY
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties (slog PROPERTIES EXPORT_NAME Slog)

install (
	DIRECTORY
		include/slog
	DESTINATION
		${CMAKE_INSTALL_INCLUDEDIR}
)

install (
	EXPORT
		slog-targets
	FILE
		SlogTargets.cmake
	NAMESPACE
		Slog::
	DESTINATION
		${INSTALL_CONFIGDIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file (
	${CMAKE_CURRENT_BINARY_DIR}/SlogConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file (
	${CMAKE_CURRENT_LIST_DIR}/cmake/SlogConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/SlogConfig.cmake
	INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install (
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/SlogConfig.cmake
		${CMAKE_CURRENT_BINARY_DIR}/SlogConfigVersion.cmake
	DESTINATION
		${INSTALL_CONFIGDIR}
)

## Export from the build tree
export (
	EXPORT
		slog-targets
	FILE
		${CMAKE_CURRENT_BINARY_DIR}/SlogTargets.cmake
	NAMESPACE Slog::
)

export (PACKAGE Slog)
