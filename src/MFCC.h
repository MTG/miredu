/*
    MIR.EDU

    A vamp plug-in library for teaching MIR.
    Copyright (C) 2013-2014 Justin Salamon.

    This file is part of MIR.EDU

    MIR.EDU is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MIR.EDU is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


    Feature info:
 
    Feature name: MFCC (Mel Frequency Cepstral Coefficients)
    Description: Compute the MFCCs of each frame of the signal.
    Unit: 
    Formula: 
    Reference: http://www.practicalcryptography.com/miscellaneous/machine-learning/guide-mel-frequency-cepstral-coefficients-mfccs/
    Credit: Implementation based on the python mfcc implementation at: https://github.com/jameslyons/python_speech_features/
 */


// Remember to use a different guard symbol in each header!
#ifndef _MFCC_H_
#define _MFCC_H_

#include <cmath>
#include <ostream>
#include <vector>
#include <vamp-sdk/Plugin.h>

using std::string;
using std::cout;
using std::endl;
using std::vector;

class MFCC : public Vamp::Plugin
{
public:
    MFCC(float inputSampleRate);
    virtual ~MFCC();

    string getIdentifier() const;
    string getName() const;
    string getDescription() const;
    string getMaker() const;
    int getPluginVersion() const;
    string getCopyright() const;

    InputDomain getInputDomain() const;
    size_t getPreferredBlockSize() const;
    size_t getPreferredStepSize() const;
    size_t getMinChannelCount() const;
    size_t getMaxChannelCount() const;

    ParameterList getParameterDescriptors() const;
    float getParameter(string identifier) const;
    void setParameter(string identifier, float value);

    ProgramList getPrograms() const;
    string getCurrentProgram() const;
    void selectProgram(string name);

    OutputList getOutputDescriptors() const;

    bool initialise(size_t channels, size_t stepSize, size_t blockSize);
    void reset();

    FeatureSet process(const float *const *inputBuffers,
                       Vamp::RealTime timestamp);

    FeatureSet getRemainingFeatures();

protected:

    float hz2mel(float hz);
    float mel2hz(float mel);
    vector< vector<float> > get_filterbanks(int nfilt, int nfft, float samplerate, float lowfreq, float highfreq);

    // plugin-specific data and methods go here
    size_t m_blockSize;
    size_t m_stepSize;

    float m_minFreq;
    float m_maxFreq;
    size_t m_nFilters;

    vector< vector<float> > m_filterbank;
};


#endif // _MFCC_H_
