#include "mfcc/WavReader.h"

using namespace std;

namespace voice {

  WavReader::WavReader(const string& filePath) {
    cout << "WavReader input WAV file: " << filePath << endl;
    file = ifstream(filePath, ios::in | ios::binary | ios::ate);
  }

  WavReader::~WavReader() {
    file.close();
  }

  void WavReader::init() {
    if (file.is_open()) {
      streampos size = file.tellg();
      cout << "Input WAV size (bytes): " << size << endl;

      file.seekg(0, ios::beg);
      file.read((char *)&header, sizeof(WavHeader));

      if (header.nChannels == 1) {
        cout << "Mono" << endl;
      } else {
        cout << "Stereo" << endl;
      }
      cout << "Sample Rate: " << header.nSamplesPerSec << endl;
      cout << "Byte Rate: " << header.nAvgBytesPerSec << endl;
      cout << "Bits Per Sample: " << header.wBitsPerSample << endl;

      samplingRate = header.nSamplesPerSec / 1000;
      frameLength *= samplingRate;
      frameSpace *= samplingRate;

      bufferSize = sizeof(short) * frameLength;

      cout << "Sampling rate (kHz): " << samplingRate << endl;
      cout << "FrameLength: " << frameLength << endl;
      cout << "FrameSpace: " << frameSpace << endl;
    } else {
      cout << "Initilization failed." << endl;
    }
  }

  bool WavReader::isInitialized() {
    return file.is_open();
  }

  int WavReader::getSamplingLength() {
    return samplingRate;
  }

  int WavReader::getFrameLength() {
    return frameLength;
  }

  int WavReader::read(vector<short>& buffer) {
    buffer.resize(frameLength);

    file.read((char*)&buffer[0], bufferSize);
    if (file.gcount() == bufferSize) {
      return bufferSize;
    } else {
      cout << "Frame incomplete: " << file.gcount() << " bytes. Return 0 instead." << endl;
      buffer.clear();
      return 0;
    }
  }
} // namespace voice
