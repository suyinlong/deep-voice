#ifndef VOICE_MFCC_WAVREADER_H
#define VOICE_MFCC_WAVREADER_H

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

} // namespace voice

#endif // VOICE_MFCC_WAVREADER_H
