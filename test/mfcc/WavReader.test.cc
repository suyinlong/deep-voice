#include <iostream>
#include "gtest/gtest.h"
#include "MfccTestConfig.h"
#include "mfcc/WavReader.h"

using namespace voice;

TEST(WavReaderTest, Initilization) {
  WavReader reader(VOICE_MFCC_FILEPATH);
  reader.init();
  ASSERT_TRUE(reader.isInitialized());
  EXPECT_EQ(550, reader.getFrameLength());
  EXPECT_EQ(22, reader.getSamplingLength());
}

TEST(WavReaderTest, InitilizationFailed) {
  WavReader reader("");
  reader.init();
  ASSERT_FALSE(reader.isInitialized());
}

TEST(WavReaderTest, ReadFrame) {
  WavReader reader(VOICE_MFCC_FILEPATH);
  reader.init();
  EXPECT_EQ(0, reader.read().size());
}
