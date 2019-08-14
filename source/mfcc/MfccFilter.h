#ifndef VOICE_MFCC_MFCCFILTER_H_
#define VOICE_MFCC_MFCCFILTER_H_

#include <cmath>
#include <vector>

namespace voice {
  class MfccFilter {
  public:
    explicit MfccFilter(int samplingRate, int frameLength);
    ~MfccFilter();
    void init();
    int getSamplingRate() const;
    int getFrameLength() const;
    int getHighLimit() const;
    int getLowLimit() const;
    int getFilterNumber() const;
    unsigned long getFftLength() const;
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

#endif // VOICE_MFCC_MFCCFILTER_H_
