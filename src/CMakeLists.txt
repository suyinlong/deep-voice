cmake_minimum_required (VERSION 3.0)
project (voice C CXX)

set (PROJECT_VERSION_MAJOR 1)
set (PROJECT_VERSION_MINOR 0)

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "Release")
endif()

set (CXX_FLAGS
  -g
  -std=c++11
  )
string (REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set (CMAKE_CXX_FLAGS_DEBUG "-O0")
set (CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories (${PROJECT_SOURCE_DIR})

string (TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message (STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

include_directories (${PROJECT_SOURCE_DIR}/mfcc)
add_subdirectory (mfcc)
set (EXTRA_LIBS ${EXTRA_LIBS} deep-voice-mfcc)

add_executable (voice voice.cc)
target_link_libraries (voice ${EXTRA_LIBS})

enable_testing()
