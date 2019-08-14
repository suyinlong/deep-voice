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

function (testCase TEST_NAME EXEC_NAME SRC_NAME)
  add_executable (${EXEC_NAME} ${SRC_NAME})
  target_link_libraries (${EXEC_NAME} ${EXTRA_LIBS})
  add_test (NAME ${TEST_NAME} COMMAND ${EXEC_NAME})
endfunction()

testCase (WavReaderTest wav-reader-test WavReader.test.cc)
testCase (MfccFilterTest mfcc-filter-test MfccFilter.test.cc)
testCase (MfccProcessorTest mfcc-processor-test MfccProcessor.test.cc)
