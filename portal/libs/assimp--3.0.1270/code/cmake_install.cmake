# Install script for directory: /media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264")
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.3.0.1264"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/lib/libassimp.so.3.0.1264"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/lib/libassimp.so.3"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/lib/libassimp.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.3.0.1264"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264-dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/anim.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/ai_assert.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/camera.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/color4.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/color4.inl"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/config.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/defs.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/cfileio.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/light.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/material.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/material.inl"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/matrix3x3.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/matrix3x3.inl"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/matrix4x4.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/matrix4x4.inl"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/mesh.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/postprocess.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/quaternion.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/quaternion.inl"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/scene.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/texture.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/types.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/vector2.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/vector2.inl"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/vector3.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/vector3.inl"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/version.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/cimport.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/importerdesc.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/Importer.hpp"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/DefaultLogger.hpp"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/ProgressHandler.hpp"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/IOStream.hpp"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/IOSystem.hpp"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/Logger.hpp"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/LogStream.hpp"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/NullLogger.hpp"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/cexport.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/Exporter.hpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264-dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264-dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/Compiler/pushpack1.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/../include/assimp/Compiler/poppack1.h"
    "/media/Traveler/Projects/rmit-rendering/portal/libs/assimp--3.0.1270/code/pstdint.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264-dev")

