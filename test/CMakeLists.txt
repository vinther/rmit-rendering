set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/test)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test)

set(TEST_ENABLED OFF CACHE BOOL "Enable building of test binaries")

set(TEST_FILE_SYSTEM OFF CACHE BOOL "Build test binary for the file system subsystem")
set(TEST_DATA_STRUCTURES OFF CACHE BOOL "Build test binary for data structures")

if (${TEST_ENABLED})
	find_package(Boost COMPONENTS unit_test_framework REQUIRED)

	if (${TEST_FILE_SYSTEM})
		add_subdirectory(file_system)
	endif()
	
	if (${TEST_DATA_STRUCTURES})
		add_subdirectory(data_structures)
	endif()
endif()