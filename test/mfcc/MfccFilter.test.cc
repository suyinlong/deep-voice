#include "mfcc/MfccFilter.h"

#include <cmath>

#include "gtest/gtest.h"
#include "MfccTestConfig.h"

using namespace std;

namespace voice {

  const int samplingRate = 44;
  const int frameLength = 1100;

  TEST(MfccFilterTest, Initilization) {
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
      ASSERT_NEAR(sum, 1.0, 1e-6);
    }
  }

} // namespace voice
