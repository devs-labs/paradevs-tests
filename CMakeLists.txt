CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(PARADEVS_TEST CXX C)
ENABLE_TESTING()

SET(MODEL_MAJOR 0)
SET(MODEL_MINOR 1)
SET(MODEL_PATCH 0)
SET(MODEL_EXTRA "-dev")

IF ("${MODEL_EXTRA}" STREQUAL "")
  SET(MODEL_NAME "${PROJECT_NAME}-${MODEL_MAJOR}.${MODEL_MINOR}.${MODEL_PATCH}")
ELSE ("${MODEL_EXTRA}" STREQUAL "")
  SET(MODEL_NAME "${PROJECT_NAME}-${MODEL_MAJOR}.${MODEL_MINOR}.${MODEL_PATCH}${MODEL_EXTRA}")
ENDIF ("${MODEL_EXTRA}" STREQUAL "")

#
 # CPack configuration
#

INCLUDE(CMakeCPack.cmake)

#
 # Debug mode
#

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed -Wall -Wextra -std=c++11")
  IF (UNIX)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
  ENDIF (UNIX)
ENDIF ()

if (CMAKE_BUILD_TYPE STREQUAL "")
  SET(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build" FORCE)
endif ()

#
 # Thread
#

message(STATUS "checking for a (p)thread library")
find_package(Threads REQUIRED)
if (NOT Threads_FOUND)
  message(ERROR " failed to found a (p)thread support.")
else ()
  message(STATUS " threads support enabled (${CMAKE_THREAD_LIBS_INIT})")
endif ()

#
 # Modules
#

INCLUDE(CheckIncludeFileCXX)
INCLUDE(CheckIncludeFile)
INCLUDE(CheckLibraryExists)
INCLUDE(CMakeDetermineCCompiler)

#
 # Check libraries with pkgconfig
#

FIND_PACKAGE(PkgConfig REQUIRED)

# FIXME use old-style link directories for now
IF (COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 OLD)
ENDIF (COMMAND CMAKE_POLICY)

PKG_CHECK_MODULES(PARADEVS REQUIRED paradevs-1.0)
PKG_CHECK_MODULES(LIBXML REQUIRED libxml-2.0)

#
 # Test the libboost header and libboost-text library.
#

OPTION(WITH_TEST "use the libboost test unit library [default: on]" ON)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_NO_SYSTEM_PATHS OFF)
SET(Boost_USE_STATIC_RUNTIME ON)
IF (WIN32)
  SET(Boost_THREADAPI "win32")
ENDIF (WIN32)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR})

find_package(MPI REQUIRED)
if (NOT MPI_CXX_FOUND)
  message(ERROR " failed to found a C++ compiler with MPI support.")
else()
  message(STATUS " MPI support enabled")
  include_directories(${MPI_CXX_INCLUDE_PATH})
endif ()

message(STATUS "checking for Boost headers and libraries")
find_package(Boost 1.55 REQUIRED COMPONENTS graph mpi timer serialization
  system)
if (NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "The boost graph library is required")
else ()
  message(STATUS " Boost found")
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif ()

#
 # Generate the config.h
#

CHECK_INCLUDE_FILE_CXX(cxxabi.h PARADEVS_HAVE_GCC_ABI_DEMANGLE)
CHECK_INCLUDE_FILE_CXX(execinfo.h PARADEVS_HAVE_EXECINFO)
CHECK_INCLUDE_FILE_CXX(signal.h PARADEVS_HAVE_SIGNAL)

#
 # Browse the src subdirectory
#

ADD_SUBDIRECTORY(src)

#
 # Status
#

MESSAGE(STATUS "- - - -")
MESSAGE(STATUS "${PARADEVS_NAME_COMPLETE} configured successfully")
MESSAGE(STATUS "Using ${CMAKE_INSTALL_PREFIX} for installation")
MESSAGE(STATUS "Build type ${CMAKE_BUILD_TYPE}")
IF (PARADEVS_HAVE_GCC_ABI_DEMANGLE)
  MESSAGE(STATUS "Build with GCC ABI Demangle...: yes")
ENDIF (PARADEVS_HAVE_GCC_ABI_DEMANGLE)
IF (PARADEVS_HAVE_EXECINFO)
  MESSAGE(STATUS "Build with execinfo.h.........: yes")
ENDIF (PARADEVS_HAVE_EXECINFO)
IF (Boost_FOUND)
  MESSAGE(STATUS "Build with boost..............: yes")
ENDIF (Boost_FOUND)
MESSAGE(STATUS "- - - -")

# vim:tw=0:ts=8:tw=0:sw=2:sts=2
