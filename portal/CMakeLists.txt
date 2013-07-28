cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLM REQUIRED)
find_package(SDL REQUIRED)
find_package(Assimp REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS                "-Wall -Werror -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else ()
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ()

include_directories(${GLM_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE PORTAL_SOURCES src/*.cpp)

add_executable (portal ${PORTAL_SOURCES})

target_link_libraries(portal 
	${SDL_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${ASSIMP_LIBRARIES})
