#include "mfcc/MfccProcessor.h"
#include <iostream>
#include "gtest/gtest.h"
#include "MfccTestConfig.h"

using namespace std;

namespace voice {

  const int samplingRate = 44;
  const int frameLength = 1100;

  TEST(MfccProcessorTest, Initialization) {
    MfccFilter mfccFilter(samplingRate, frameLength);
    MfccProcessor mfccProcessor_0(mfccFilter, frameLength);
    MfccProcessor mfccProcessor_1(mfccFilter);
  }

  TEST(MfccProcessorTest, InitHamming) {
    MfccFilter mfccFilter(samplingRate, 5);
    MfccProcessor mfccProcessor(mfccFilter, 5);

    vector<double> hamming;
    mfccProcessor.initHamming(hamming);

    // Length = 5, Hamming window = {0.08, 0.54, 1.00, 0.54, 0.08}
    EXPECT_EQ(5, hamming.size());
    ASSERT_NEAR(hamming[0], 0.08, VOICE_MFCC_DELTA);
    ASSERT_NEAR(hamming[1], 0.54, VOICE_MFCC_DELTA);
    ASSERT_NEAR(hamming[2], 1.00, VOICE_MFCC_DELTA);
    ASSERT_NEAR(hamming[3], 0.54, VOICE_MFCC_DELTA);
    ASSERT_NEAR(hamming[4], 0.08, VOICE_MFCC_DELTA);
  }

  TEST(MfccProcessorTest, ProcessHamming) {
    MfccFilter mfccFilter(samplingRate, 5);
    MfccProcessor mfccProcessor(mfccFilter, 5);

    vector<short> frame = {267, 30, 19845, 0, 94};
    vector<double> hamming = {0.08, 0.54, 1.00, 0.54, 0.08};
    vector<float> buffer;
    mfccProcessor.processHamming(frame, hamming, buffer);

    EXPECT_EQ(5, buffer.size());
    ASSERT_NEAR(buffer[0], 21.36, VOICE_MFCC_DELTA);
    ASSERT_NEAR(buffer[1], 16.2, VOICE_MFCC_DELTA);
    ASSERT_NEAR(buffer[2], 19845.0, VOICE_MFCC_DELTA);
    ASSERT_NEAR(buffer[3], 0.0, VOICE_MFCC_DELTA);
    ASSERT_NEAR(buffer[4], 7.52, VOICE_MFCC_DELTA);
  }

  TEST(MfccProcessorTest, ProcessComplex) {
    MfccFilter mfccFilter(samplingRate, 5);
    MfccProcessor mfccProcessor(mfccFilter, 5);

    vector<float> buffer = {21.36, 16.2, 19845.0, 0.0, 7.52};
    vector<complex<float>> fft;
    mfccProcessor.processComplex(10, buffer, fft);

    EXPECT_EQ(10, fft.size());
    ASSERT_NEAR(fft[0].real(), 21.36, VOICE_MFCC_DELTA);
    ASSERT_NEAR(fft[1].real(), 16.2, VOICE_MFCC_DELTA);
    ASSERT_NEAR(fft[2].real(), 19845.0, VOICE_MFCC_DELTA);
    ASSERT_NEAR(fft[3].real(), 0.0, VOICE_MFCC_DELTA);
    ASSERT_NEAR(fft[4].real(), 7.52, VOICE_MFCC_DELTA);
    for (int i = 5; i < 10; i++) ASSERT_NEAR(fft[i].real(), 0.0, VOICE_MFCC_DELTA);
    for (int i = 0; i < 10; i++) ASSERT_NEAR(fft[i].imag(), 0.0, VOICE_MFCC_DELTA);
  }

  // TEST FFT

  TEST(MfccProcessorTest, ComputeMagSquare) {
    MfccFilter mfccFilter(samplingRate, 5);
    MfccProcessor mfccProcessor(mfccFilter, 5);

    vector<complex<float>> fft = { {3.0, 4.0}, {1.0, 2.5}};
    vector<float> mag;
    mfccProcessor.computeMagSquare(fft, mag);

    EXPECT_EQ(2, mag.size());
    ASSERT_NEAR(mag[0], 25.0, VOICE_MFCC_DELTA);
    ASSERT_NEAR(mag[1], 7.25, VOICE_MFCC_DELTA);
  }

  TEST(MfccPrcessorTest, ComputeMelEnergy) {
    MfccFilter mfccFilter(samplingRate, 5);
    MfccProcessor mfccProcessor(mfccFilter, 5);

    vector<vector<float>> filterWeight = {{0.0, 1.0}, {0.25, 0.75}, {0.5, 0.5}, {0.75, 0.25}, {1.0, 0.0}};
    vector<float> mag = {25.0, 7.25};
    vector<float> melEnergy;
    mfccProcessor.computeMelEnergy(filterWeight, mag, melEnergy);

    EXPECT_EQ(5, melEnergy.size());
    ASSERT_NEAR(melEnergy[0], 1.981001469, VOICE_MFCC_DELTA);
    ASSERT_NEAR(melEnergy[1], 2.458519895, VOICE_MFCC_DELTA);
    ASSERT_NEAR(melEnergy[2], 2.780370863, VOICE_MFCC_DELTA);
    ASSERT_NEAR(melEnergy[3], 3.023469029, VOICE_MFCC_DELTA);
    ASSERT_NEAR(melEnergy[4], 3.218875825, VOICE_MFCC_DELTA);
  }

  TEST(MfccProcessorTest, ComputeCepstrum) {
    MfccFilter mfccFilter(samplingRate, 5);
    MfccProcessor mfccProcessor(mfccFilter, 5);

    vector<float> melEnergy = {1.981001469, 2.458519895, 2.780370863, 3.023469029, 3.218875825};
    vector<float> coeff;
    mfccProcessor.computeCepstrum(melEnergy, coeff);

    EXPECT_EQ(12, coeff.size());
    ASSERT_NEAR(coeff[0], 14.694505647, VOICE_MFCC_DELTA);
    ASSERT_NEAR(coeff[1], 11.888104792, VOICE_MFCC_DELTA);
    ASSERT_NEAR(coeff[2], 4.540851969, VOICE_MFCC_DELTA);
    ASSERT_NEAR(coeff[3], -4.540851969, VOICE_MFCC_DELTA);
    ASSERT_NEAR(coeff[4], -11.888104792, VOICE_MFCC_DELTA);
  }

} // namespace voice
