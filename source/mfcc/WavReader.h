#ifndef VOICE_MFCC_WAVREADER_H_
#define VOICE_MFCC_WAVREADER_H_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace voice {

  // http://soundfile.sapp.org/doc/WaveFormat/
  struct WavHeader {
    int rId;
    int rLen;
    int wId;
    int fId;

    int fLen;

    short wFormatTag;
    short nChannels;
    int nSamplesPerSec; // Sampling rate
    int nAvgBytesPerSec;
    short nBlockAlign;
    short wBitsPerSample;
    int dId;
    int wSampleLength;
  };

  class WavReader {
  public:
    explicit WavReader(const std::string& filePath);
    ~WavReader();
    void init();
    bool isInitialized();
    int getSamplingLength();
    int getFrameLength();
    int read(std::vector<short>& buffer);
  private:
    std::ifstream file;
    WavHeader header;

    int samplingRate = 0; // sampling rate in KHz
    int frameLength = 25; // Frame length in ms
    int frameSpace = 10; // Frame space in ms

    int bufferSize = sizeof(short) * frameLength; // Frame buffer size in bytes
  };

} // namespace voice

#endif // VOICE_MFCC_WAVREADER_H_
