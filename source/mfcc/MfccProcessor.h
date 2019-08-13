#ifndef VOICE_MFCC_MFCCPROCESSOR_H_
#define VOICE_MFCC_MFCCPROCESSOR_H_

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

namespace voice {
  class MfccProcessor {
  public:
    explicit MfccProcessor(int frameLength);
    ~MfccProcessor();
  private:
    static constexpr float PI = 4.0F * std::atan(1.0F);
    static constexpr float TWO_PI = 8.0F * std::atan(1.0F);

    const int frameLength_;
    std::vector<double> hamming; // Hamming window
    std::vector<float> data; // Data

    void initHamming();
    void processHamming(std::vector<short>& frame);
    float frameEnergy();
    void computeZeroFft();
    void computeFft();
    void computeMagSquare();
    void computeMelEN();
    void computeCepstrum();

  };
} // namespace voice

#endif // VOICE_MFCC_MFCCPROCESSOR_H_
