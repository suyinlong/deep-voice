enable_testing()

set (VOICE_MFCC_FILEPATH ${CMAKE_CURRENT_SOURCE_DIR}/Void.wav)

message (STATUS "PATH = " ${VOICE_MFCC_FILEPATH})

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/MfccTestConfig.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/MfccTestConfig.h"
)

include_directories (${CMAKE_CURRENT_BINARY_DIR})

set (EXTRA_LIBS ${EXTRA_LIBS} gtest_main)
set (EXTRA_LIBS ${EXTRA_LIBS} deep-voice-mfcc)

add_executable (wav-reader-test WavReader.test.cc)
target_link_libraries (wav-reader-test ${EXTRA_LIBS})

add_test (NAME WavReaderTest COMMAND wav-reader-test)
