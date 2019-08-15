#ifndef VOICE_MFCC_MFCCPROCESSOR_H_
#define VOICE_MFCC_MFCCPROCESSOR_H_

#include <algorithm>
#include <bitset>
#include <cmath>
#include <complex>
#include <iostream>
#include <numeric>
#include <vector>

#include "mfcc/MfccFilter.h"

namespace voice {
  class MfccProcessor {
  public:
    explicit MfccProcessor(MfccFilter& mfccFilter, int frameLength);
    explicit MfccProcessor(MfccFilter& mfccFilter);
    ~MfccProcessor();
    void initHamming(std::vector<double>& hamming);
    void processHamming(std::vector<short>& frame, std::vector<double>& hamming, std::vector<float>& buffer);
    float frameEnergy(std::vector<float>& buffer);
    void processComplex(int fftLength, std::vector<float>& buffer, std::vector<std::complex<float>>& fft);
    void computeFft(std::vector<std::complex<float>>& fft);
    void computeMagSquare(std::vector<std::complex<float>>& fft, std::vector<float>& mag);
    void computeMelEnergy(std::vector<std::vector<float>>& filterWeight, std::vector<float>& mag, std::vector<float>& melEnergy);
    void computeCepstrum(std::vector<float>& melEnergy, std::vector<float>& coeff);

    template <class T>
    inline void fillVector(std::vector<T>& v, int expectedSize, T val) {
      v.resize(expectedSize);
      std::fill(v.begin(), v.end(), val);
    }

    template <class T>
    inline void iotaVector(std::vector<T>& v, int expectedSize, T val) {
      v.resize(expectedSize);
      std::iota(v.begin(), v.end(), val);
    }

  private:
    static constexpr float PI = 4.0F * std::atan(1.0F);
    static constexpr float TWO_PI = 8.0F * std::atan(1.0F);
    static const int PCEP = 12;

    const MfccFilter& mfccFilter_;
    const int frameLength_;
  };
} // namespace voice

#endif // VOICE_MFCC_MFCCPROCESSOR_H_
