#include "mfcc/MfccProcessor.h"
#include <iostream>
#include "gtest/gtest.h"
#include "MfccTestConfig.h"

using namespace std;

namespace voice {

  const int samplingRate = 44;
  const int frameLength = 1100;

  TEST(MfccProcessorTest, Initialization) {
    MfccFilter mfccFilter(samplingRate, frameLength);
    MfccProcessor mfccProcessor_0(mfccFilter, frameLength);
    MfccProcessor mfccProcessor_1(mfccFilter);
  }

  TEST(MfccProcessorTest, InitHamming) {
    MfccFilter mfccFilter(samplingRate, 5);
    MfccProcessor mfccProcessor(mfccFilter, 5);

    vector<double> hamming;
    mfccProcessor.initHamming(hamming);

    // Length = 5, Hamming window = {0.08, 0.54, 1.00, 0.54, 0.08}
    EXPECT_EQ(5, hamming.size());
    ASSERT_NEAR(hamming[0], 0.08, VOICE_MFCC_DELTA);
    ASSERT_NEAR(hamming[1], 0.54, VOICE_MFCC_DELTA);
    ASSERT_NEAR(hamming[2], 1.00, VOICE_MFCC_DELTA);
    ASSERT_NEAR(hamming[3], 0.54, VOICE_MFCC_DELTA);
    ASSERT_NEAR(hamming[4], 0.08, VOICE_MFCC_DELTA);
  }

  TEST(MfccProcessorTest, ProcessHamming) {
    MfccFilter mfccFilter(samplingRate, 5);
    MfccProcessor mfccProcessor(mfccFilter, 5);

    vector<short> frame = {267, 30, 19845, 0, 94};
    vector<double> hamming = {0.08, 0.54, 1.00, 0.54, 0.08};
    vector<float> buffer;
    mfccProcessor.processHamming(frame, hamming, buffer);

    EXPECT_EQ(5, buffer.size());
    ASSERT_NEAR(buffer[0], 21.36, VOICE_MFCC_DELTA);
    ASSERT_NEAR(buffer[1], 16.2, VOICE_MFCC_DELTA);
    ASSERT_NEAR(buffer[2], 19845.0, VOICE_MFCC_DELTA);
    ASSERT_NEAR(buffer[3], 0.0, VOICE_MFCC_DELTA);
    ASSERT_NEAR(buffer[4], 7.52, VOICE_MFCC_DELTA);
  }

} // namespace voice
