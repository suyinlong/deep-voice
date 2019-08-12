set (MFCC_SRCS
  WavReader.cc
  )

add_library (deep-voice-mfcc ${MFCC_SRCS})

install (TARGETS deep-voice-mfcc DESTINATION lib)

file (GLOB HEADERS "*.h")
install (FILES ${HEADERS} DESTINATION include/mfcc)
