#include "mfcc/MfccFilter.h"

#include <cmath>

#include "gtest/gtest.h"
#include "MfccTestConfig.h"

using namespace std;

namespace voice {

  const int samplingRate = 44;
  const int frameLength = 1100;

  TEST(MfccFilterTest, Initialization) {
    MfccFilter mfccFilter(samplingRate, frameLength);
  }

  TEST(MfccFilterTest, Getter) {
    MfccFilter mfccFilter(samplingRate, frameLength);

    EXPECT_EQ(samplingRate, mfccFilter.getSamplingRate());
    EXPECT_EQ(frameLength, mfccFilter.getFrameLength());
    EXPECT_EQ(20, mfccFilter.getHighLimit());
    EXPECT_EQ(0, mfccFilter.getLowLimit());
    EXPECT_EQ(26, mfccFilter.getFilterNumber());
    EXPECT_EQ(512, mfccFilter.getFftLength());
  }

  TEST(MfccFilterTest, FilterWeight) {
    MfccFilter mfccFilter(samplingRate, frameLength);
    auto filter = mfccFilter.getFilter();

    EXPECT_EQ(26, filter.size());
    EXPECT_EQ(257, filter[0].size());
    for (auto& row: filter) {
      float sum = 0.0;
      for (auto& val: row) {
        ASSERT_FALSE(isnan(val));
        ASSERT_FALSE(isinf(val));
        sum += val;
      }
      ASSERT_NEAR(sum, 1.0, VOICE_MFCC_DELTA);
    }
  }

} // namespace voice
