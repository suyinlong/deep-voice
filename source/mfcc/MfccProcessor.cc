#include "mfcc/MfccProcessor.h"

using namespace std;

namespace voice {

  MfccProcessor::MfccProcessor(MfccFilter& mfccFilter, int frameLength)
    : mfccFilter_(mfccFilter),
      frameLength_(frameLength) {
    initHamming();
  }

  MfccProcessor::~MfccProcessor() {
    hamming.clear();
  }

  void MfccProcessor::initHamming() {
    hamming = vector<double>(frameLength_);
    generate(hamming.begin(), hamming.end(),
      [i = 0, len = frameLength_] () mutable {
        return (float)(0.54 - 0.46 * cos((float)i++ * TWO_PI / (float)(len - 1)));
      });
  }

  void MfccProcessor::processHamming(vector<short>& frame, vector<float>& buffer) {
    buffer.resize(frameLength_, 0.0);
    generate(buffer.begin(), buffer.end(),
      [i = 0, f = frame, h = hamming] () mutable {
        return f[i] * h[i++];
      });
  }

  float MfccProcessor::frameEnergy(vector<float>& buffer) {
    return inner_product(buffer.begin(), buffer.end(), buffer.begin(), 0.0);
  }

  void MfccProcessor::processComplex(int fftLength, vector<float>& buffer, vector<complex<float>>& fft) {
    fft.resize(fftLength, complex<float>(0));
    generate(fft.begin(), fft.begin() + frameLength_,
      [i = 0, b = buffer] () mutable {
        return complex<float>(b[i++]);
      });
  }

  void MfccProcessor::computeFft(vector<complex<float>>& fft) {
    int fftLen = fft.size();
    unsigned long ulPower = 0, fftLen1 = fftLen - 1;
    while (fftLen1 > 0) {
      ulPower++;
      fftLen1 >>= 1;
    }

    bitset<sizeof(unsigned long) * 8> bsIndex;
    unsigned long ulIndex, ulK;
    for (unsigned long p = 0; p < fftLen; p++) {
      ulIndex = 0;
      ulK = 1;
      bsIndex = bitset<sizeof(unsigned long) * 8>(p);
      for (unsigned long j = 0; j < ulPower; j++) {
        ulIndex += bsIndex.test(ulPower - j - 1) ? ulK : 0;
        ulK *= 2;
      }
      if (ulIndex > p) {
        complex<float> c = fft[p];
        fft[p] = fft[ulIndex];
        fft[ulIndex] = c;
      }
    }

    vector<complex<float>> fftW;
    for (unsigned long i = 0; i < fftLen / 2; i++) {
      fftW.push_back(complex<float>(cos(2 * i * PI / fftLen), -1 * sin(2 * i * PI / fftLen)));
    }

    unsigned long ulGroupLength = 1, ulHalfLength = 0, ulGroupCount = 0;
    complex<float> cw, c1, c2;

    for (unsigned long b = 0; b < ulPower; b++) {
      ulHalfLength = ulGroupLength;
      ulGroupLength *= 2;
      for (unsigned long j = 0; j < fftLen; j += ulGroupLength) {
        for (unsigned long k = 0; k < ulHalfLength; k++) {
          cw = fftW[k * fftLen / ulGroupLength];
          c1 = fft[j + k] + cw;
          c2 = fft[j + k] - cw;
          fft[j + k] = c1;
          fft[j + k + ulHalfLength] = c2;
        }
      }
    }
  }

  void MfccProcessor::computeMagSquare(vector<complex<float>>& fft, vector<float>& mag) {
    mag.resize(fft.size());
    generate(mag.begin(), mag.end(),
      [i = 0, fft = fft] () mutable {
        return norm(fft[i++]);
      });
  }

  void MfccProcessor::computeMelEN(int filterNumber, int fftLength, vector<vector<float>>& filterWeight, vector<float>& mag, vector<float>& melEnergy) {
    melEnergy.resize(filterNumber);
    generate(melEnergy.begin(), melEnergy.end(),
      [i = 0, f = filterWeight, m = mag] () mutable {
        float e = inner_product(f[i].begin(), f[i].end(), m.begin(), 0.0);
        i++;
        return (float)(log(e));
      });
  }

  void MfccProcessor::computeCepstrum(int filterNumber, vector<float>& melEnergy, vector<float>& coeff) {
    coeff.resize(PCEP);
    generate(coeff.begin(), coeff.end(),
      [i = 0, n = filterNumber, e = melEnergy] () mutable {
        float t = 0.0;
        for (int j = 0; j < n; j++) {
          t += e[j] * cos(PI * ((float) i) / (float) n) * ((float) j - 0.5F);
        }
        t = sqrt(2.0 / (float) n) * t;
        i++;
        return t;
      });
  }

} // namespace voice
