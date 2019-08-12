enable_testing()

set (EXTRA_LIBS ${EXTRA_LIBS} deep-voice-mfcc)

add_executable (wav-reader-test WavReader.test.cc)
target_link_libraries (wav-reader-test ${EXTRA_LIBS})

add_test (NAME WavReaderTest COMMAND wav-reader-test)
