#include "mfcc/MfccFilter.h"

using namespace std;

namespace voice {

  MfccFilter::MfccFilter(int samplingRate, int frameLength)
    : samplingRate_(samplingRate),
      frameLength_(frameLength),
      highLimit_(20 > (int)(samplingRate / 2) ? (int)(samplingRate / 2) : 20),
      lowLimit_(0 > highLimit_ ? (int)(highLimit_ / 2) : 0) {
    initFilter();
    createFilter(samplingRate_ * 1000, highLimit_ * 1000, lowLimit_ * 1000);
  }

  MfccFilter::~MfccFilter() {
    for (vector<float> row: filterWeight) {
      row.clear();
    }
    filterWeight.clear();
  }

  void MfccFilter::initFilter() {
    filterWeight = vector<vector<float>>(FILTER_NUM, vector<float>(FFT_LEN / 2 + 1, 0.0));
  }

  void MfccFilter::createFilter(int rate, int high, int low) {
    float df = (float) rate / (float) FFT_LEN; // This is FFT interval
    int indexLow = round((float) FFT_LEN * (float) low / (float) rate); // FFT index of low frequency limit
    int indexHigh = round((float) FFT_LEN * (float) high / (float) rate); // FFT index of high frequency limit

    float melMin = 2595.0 * log10(1.0 + (float) low / 700.0); // Mel low frequency
    float melMax = 2595.0 * log10(1.0 + (float) high / 700.0); // Mel high frequency
    float melInc = (melMax - melMin) / (float) (FILTER_NUM + 1); // Mel half bandwidth

    float melCenters[FILTER_NUM]; // Mel center frequencies
    float fCenters[FILTER_NUM]; // Hertz center frequencies

    int indexCenter[FILTER_NUM]; // FFT index for Hertz centers
    int indexStart[FILTER_NUM]; // FFT index for the first sample of each filter
    int indexStop[FILTER_NUM]; // FFT index for the last sample of each filter

    float increment, decrement; // increment and decrement of the left and right ramp

    float sum = 0.0;
    int i, j;

    for (i = 1; i <= FILTER_NUM; i++) {
      melCenters[i-1] = (float) i * melInc + melMin; // Mel center frequencies
      fCenters[i-1] = 700.0 * (pow(10.0, melCenters[i-1] / 2595.0) - 1.0); // Hertz center frequencies
      indexCenter[i-1] = round(fCenters[i-1] / df); // FFT index for Hertz centers
    }

    for (i = 1; i <= FILTER_NUM - 1; i++) {
      indexStart[i] = indexCenter[i-1]; // Start FFT index of the channel
      indexStop[i-1] = indexCenter[i]; // End FFT index of the channel
    }
    indexStart[0] = indexLow;
    indexStop[FILTER_NUM - 1] = indexHigh;

    for (i = 1; i <= FILTER_NUM; i++) {
      increment = 1.0 / ((float) indexCenter[i-1] - (float) indexStart[i-1]); // left ramp
      for (j = indexStart[i-1]; j <= indexCenter[i-1]; j++) {
        filterWeight[i-1][j] = ((float) j - (float) indexStart[i-1]) * increment;
      }
      decrement = 1.0 / ((float) indexStop[i-1] - (float) indexCenter[i-1]); // right ramp
      for (j = indexCenter[i-1]; j <= indexStop[i-1]; j++) {
        filterWeight[i-1][j] = 1.0 - ((float) j - (float) indexCenter[i-1]) * decrement;
      }
    }

    for (i = 1; i <= FILTER_NUM; i++) {
      for (j = 1; j <= FFT_LEN / 2 + 1; j++) {
        sum = sum + filterWeight[i-1][j-1];
      }
      for (j = 1; j <= FFT_LEN / 2 + 1; j++) {
        filterWeight[i-1][j-1] /= sum; // Normalize filter weights by sum
      }
      sum = 0.0;
    }
  }

  int MfccFilter::getSamplingRate() const {
    return samplingRate_;
  }

  int MfccFilter::getFrameLength() const {
    return frameLength_;
  }

  int MfccFilter::getHighLimit() const {
    return highLimit_;
  }

  int MfccFilter::getLowLimit() const {
    return lowLimit_;
  }

  int MfccFilter::getFilterNumber() const {
    return FILTER_NUM;
  }

  unsigned long MfccFilter::getFftLength() const {
    return FFT_LEN;
  }

  std::vector<std::vector<float>>& MfccFilter::getFilter() {
    return filterWeight;
  }

} // namespace voice
