#ifndef ESSENTIA_AUDIO2PITCH_H
#define ESSENTIA_AUDIO2PITCH_H

#include "algorithmfactory.h"

namespace essentia {
namespace standard {

class Audio2Pitch : public Algorithm {

  protected: 
    Input<std::vector<Real>> _frame;
    Output<Real> _pitch;
    Output<Real> _pitchConfidence;
    Output<Real> _loudness;
    Output<int> _voiced;

    Algorithm* _pitchAlgorithm;
    Algorithm* _loudnessAlgorithm;
    // auxiliary algorithms for FFT-based pitch
    Algorithm* _windowing;
    Algorithm* _spectrum;

    Real _sampleRate;
    int _frameSize;
    Real _minFrequency;
    Real _maxFrequency;
    std::string _pitchAlgorithmName;
    std::string _loudnessAlgorithmName;
    Real _tolerance;
    Real _pitchConfidenceThreshold;
    Real _loudnessThreshold;

    bool _isSpectral;
    
    bool isAboveThresholds(Real pitchConfidence, Real loudness);

  public:
    Audio2Pitch() {
      declareInput(_frame, "frame", "the input frame to analyse");
      declareOutput(_pitch, "pitch", "detected pitch in Hz");
      declareOutput(_pitchConfidence, "pitchConfidence", "confidence of detected pitch from 0.0 - 1.0");
      declareOutput(_loudness, "loudness", "detected loudness in dBFS");
      declareOutput(_voiced, "voiced", "voiced frame categorization, 1 for voiced and 0 for unvoiced frame");

      _isSpectral = true;
    }

    ~Audio2Pitch() {
      if (_pitchAlgorithm) delete _pitchAlgorithm;
      if (_loudnessAlgorithm) delete _loudnessAlgorithm;
      if (_windowing) delete _windowing;
      if (_spectrum) delete _spectrum;
    }

    void declareParameters() {
      declareParameter("sampleRate", "sample rate of incoming audio frames", "[8000,inf)", 44100);
      declareParameter("frameSize", "size of input frame in samples", "[1,inf)", 1024);
      declareParameter("minFrequency", "minimum frequency to detect in Hz", "[10,20000]", 60.f);
      declareParameter("maxFrequency", "maximum frequency to detect in Hz", "[10,20000]", 2300.f);
      declareParameter("pitchAlgorithm", "pitch algorithm to use", "{pyin,pyin_fft}", "pyin_fft");
      declareParameter("loudnessAlgorithm", "loudness algorithm to use", "{loudness,rms}", "rms");
      declareParameter("weighting", "string to assign a weighting function", "{default,A,B,C,D,Z}", "default");
      declareParameter("tolerance", "sets tolerance for peak detection on pitch algorithm", "[0,1]", 1.0f);
      declareParameter("pitchConfidenceThreshold", "level of pitch confidence above which note ON/OFF start to be considered", "[0,1]", 0.25);
      declareParameter("loudnessThreshold", "loudness level above which note ON/OFF start to be considered, in linear values", "[0,1]", 0.0031); // ~ -50dB
    }

    void configure();
    void compute();

    static const char* name;
    static const char* category;
    static const char* description;
};

} // namespace standard
} // namespace essentia

#endif