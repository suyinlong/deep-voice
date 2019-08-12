#include "mfcc/MfccProcessor.h"

using namespace std;
using namespace voice;

MfccProcessor::MfccProcessor(int frameLength)
  : frameLength_(frameLength) {
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

void MfccProcessor::processHamming(vector<short>& frame) {
  data = vector<float>(frameLength_);
  generate(data.begin(), data.end(),
    [i = 0, f = frame, h = hamming] () mutable {
      return f[i] * h[i++];
    });
}

float MfccProcessor::frameEnergy() {
  return inner_product(data.begin(), data.end(), data.begin(), 0.0);
}
