cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLM REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(JPEG REQUIRED)
find_package(Assimp REQUIRED)
find_package(Boost COMPONENTS filesystem system REQUIRED)

include_directories(${GLM_INCLUDE_DIRS})
include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2IMAGE_INCLUDE_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -Werror -Wextra -std=c++11 -fshow-source-location")

add_definitions(-DGLM_SIMD_ENABLE_XYZW_UNION)

# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	execute_process(
		COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	if (NOT(GCC_VERSION VERSION_GREATER 4.6 OR GCC_VERSION VERSION_EQUAL 4.6))
	   message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.6 or greater.")
	endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
else ()
	message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ()

MACRO (TODAY RESULT)
    IF (WIN32)
        EXECUTE_PROCESS(COMMAND "cmd" " /C date /T" OUTPUT_VARIABLE ${RESULT})
        string(REGEX REPLACE "(..)/(..)/..(..).*" "\\1/\\2/\\3" ${RESULT} ${${RESULT}})
    ELSEIF(UNIX)
        EXECUTE_PROCESS(COMMAND "date" "+%d/%m/%Y" OUTPUT_VARIABLE ${RESULT})
        string(REGEX REPLACE "(..)/(..)/..(..).*" "\\1/\\2/\\3" ${RESULT} ${${RESULT}})
    ELSE (WIN32)
        MESSAGE(SEND_ERROR "date not implemented")
        SET(${RESULT} 000000)
    ENDIF (WIN32)
ENDMACRO (TODAY)

include_directories(${PROJECT_SOURCE_DIR}/include)
configure_file(${PROJECT_SOURCE_DIR}/include/Config.cmake.hpp ${PROJECT_SOURCE_DIR}/include/Config.hpp)

file(GLOB_RECURSE CLIENT_SOURCES src/*/*.c src/*/*.cpp src/*/*.cxx)

add_library (client ${CLIENT_SOURCES})

target_link_libraries(
	client 
	${SDL2_LIBRARY} 
	${SDL2IMAGE_LIBRARY} 
	${SDL2TTF_LIBRARY} 
	${JPEG_LIBRARY} 
	${OPENGL_LIBRARIES} 
	${GLUT_LIBRARY} 
	${GLEW_LIBRARIES} 
	${ASSIMP_LIBRARIES}
	${Boost_LIBRARIES}
	)
	
file(GLOB_RECURSE PORTAL_SOURCES src/Portal.cpp)	

add_executable (portal src/Portal.cpp)

target_link_libraries(portal client)

add_subdirectory(profiling)
add_subdirectory(test)

add_custom_target(assets
    COMMAND python ${PROJECT_SOURCE_DIR}/scripts/data_packer/pack_directory.py ${PROJECT_SOURCE_DIR}/assets -o ${PROJECT_SOURCE_DIR}/data/assets -v
)
