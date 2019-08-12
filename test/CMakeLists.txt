cmake_minimum_required (VERSION 3.0)
project (deep-voice-test C CXX)

include_directories (${PROJECT_SOURCE_DIR})

include_directories (${PROJECT_SOURCE_DIR}/mfcc)
add_subdirectory (mfcc)
