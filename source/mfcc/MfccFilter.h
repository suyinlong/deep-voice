#ifndef VOICE_MFCC_MFCCFILTER_H
#define VOICE_MFCC_MFCCFILTER_H

#include <cmath>
#include <vector>

namespace voice {
  class MfccFilter {
  public:
    explicit MfccFilter(int samplingRate, int frameLength);
    ~MfccFilter();
    void init();
    std::vector<std::vector<float>>& getFilter();
  private:
    static const int FILTER_NUM = 26; // Number of filters
    static const unsigned long FFT_LEN = 512; // FFT points

    const int samplingRate_;
    const int frameLength_;
    const int highLimit_;
    const int lowLimit_;
    std::vector<std::vector<float>> filterWeight; // Mel filterbank weights

    void initFilter();
    void createFilter(int rate, int high, int low);
  };
} // namespace voice

#endif // VOICE_MFCC_MFCCFILTER_H
