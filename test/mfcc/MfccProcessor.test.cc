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
    ASSERT_NEAR(hamming[0], 0.08, 1e-6);
    ASSERT_NEAR(hamming[1], 0.54, 1e-6);
    ASSERT_NEAR(hamming[2], 1.00, 1e-6);
    ASSERT_NEAR(hamming[3], 0.54, 1e-6);
    ASSERT_NEAR(hamming[4], 0.08, 1e-6);
  }

} // namespace voice
