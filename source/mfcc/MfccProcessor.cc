#include "mfcc/MfccProcessor.h"

using namespace std;

namespace voice {

  MfccProcessor::MfccProcessor(MfccFilter& mfccFilter, int frameLength)
    : mfccFilter_(mfccFilter),
      frameLength_(frameLength) {
  }

  MfccProcessor::MfccProcessor(MfccFilter& mfccFilter)
    : mfccFilter_(mfccFilter),
      frameLength_(mfccFilter.getFrameLength()) {
  }

  MfccProcessor::~MfccProcessor() {
  }

  void MfccProcessor::process(WavReader reader) {
    vector<double> hamming;
    vector<short> frame;
    vector<complex<float>> fft;
    vector<float> buffer, mag, melEnergy, coeff;

    initHamming(hamming);
    while (reader.read(frame) > 0) {
      processHamming(buffer, hamming, buffer);
      flaot energy = frameEnergy(buffer);
      processComplex(mfccFilter_.getFftLength(), buffer, fft);
      computeFft(fft);
      computeMagSquare(fft, mag);
      computeMelEnergy(mfccFilter.getFilter(), mag, melEnergy);
      computeCepstrum(melEnergy, coeff);
    }
  }

  void MfccProcessor::initHamming(vector<double>& hamming) {
    hamming.resize(frameLength_);
    generate(hamming.begin(), hamming.end(),
      [&, i = 0] () mutable {
        return (float)(0.54 - 0.46 * cos((float)i++ * TWO_PI / (float)(frameLength_ - 1)));
      });
  }

  void MfccProcessor::processHamming(vector<short>& frame, vector<double>& hamming, vector<float>& buffer) {
    buffer.resize(frameLength_, 0.0);
    generate(buffer.begin(), buffer.end(),
      [&, i = 0] () mutable {
        return frame[i] * hamming[i++];
      });
  }

  float MfccProcessor::frameEnergy(vector<float>& buffer) {
    return inner_product(buffer.begin(), buffer.end(), buffer.begin(), 0.0);
  }

  void MfccProcessor::processComplex(int fftLength, vector<float>& buffer, vector<complex<float>>& fft) {
    fft.resize(fftLength, complex<float>(0));
    generate(fft.begin(), fft.begin() + frameLength_,
      [&, i = 0] () mutable {
        return complex<float>(buffer[i++]);
      });
  }

  void MfccProcessor::computeFft(vector<complex<float>>& fft) {
    int fftLen = fft.size();
    unsigned long ulPower = (fftLen == 1) ? 0 : log2(fftLen - 1) + 1;

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
      [&, i = 0] () mutable {
        return norm(fft[i++]);
      });
  }

  void MfccProcessor::computeMelEnergy(vector<vector<float>>& filterWeight, vector<float>& mag, vector<float>& melEnergy) {
    melEnergy.resize(filterWeight.size());
    generate(melEnergy.begin(), melEnergy.end(),
      [&, i = 0] () mutable {
        float e = inner_product(filterWeight[i].begin(), filterWeight[i].end(), mag.begin(), 0.0);
        i++;
        return (float)(log(e));
      });
  }

  void MfccProcessor::computeCepstrum(vector<float>& melEnergy, vector<float>& coeff) {
    int filterNumber = melEnergy.size();
    vector<float> ee(melEnergy);
    for_each(ee.begin(), ee.end(),
      [j = 0] (float &e) mutable {
        e *= ((float) j++ - 0.5F);
      });

    float coeffMultiplifer = sqrt(2.0 / (float) filterNumber);
    coeff.resize(PCEP);
    generate(coeff.begin(), coeff.end(),
      [&, i = 0] () mutable {
        float co = cos(PI * ((float) i) / (float) filterNumber);
        float t = accumulate(ee.begin(), ee.end(), 0.0F,
          [=] (float& a, float& b) {
            return a + co * b;
          });
        i++;
        return coeffMultiplifer * t;
      });
  }

} // namespace voice
