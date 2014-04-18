/**
 * @file MFCC.h
 * @author  Justin Salamon <justin.salamon@nyu.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * MIR.EDU
 * 
 * A vamp plug-in library for teaching MIR.
 * Copyright (C) 2013-2014 Justin Salamon.
 *
 * MIR.EDU is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MIR.EDU is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * Feature name: MFCC (Mel Frequency Cepstral Coefficients) <br/>
 * Description: Compute the MFCCs of each frame of the signal. <br/>
 * Unit: see reference <br/>
 * Formula: see reference <br/>
 * Reference1: T. Ganchev, N. Fakotakis, and G. Kokkinakis. Comparative evaluation of various MFCC implementations on 
 * the speaker verification task. In 10th Int. Conf. on Speech and Computer, volume 1, pages 191–194, Patras, Greece, 
 * Oct. 2005. <br/>
 * Reference2: 
 * http://www.practicalcryptography.com/miscellaneous/machine-learning/guide-mel-frequency-cepstral-coefficients-mfccs/ <br/>
 * Reference3:: Implementation based on the python mfcc implementation at: 
 * https://github.com/jameslyons/python_speech_features/ <br/>
 * Reference4: The liftering function is based on the matlab implementation at (without the HTK liftering):
 * http://labrosa.ee.columbia.edu/matlab/rastamat/ <br/>
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
    vector<float> dct(vector<float> x);
    vector<float> lifter(vector<float> cep, float lift_exp);

    // plugin-specific data and methods go here
    size_t m_blockSize;
    size_t m_stepSize;

    float m_minFreq; // minimum frequency to use in MFCC computation
    float m_maxFreq; // maximum frequency to use in MFCC computation
    size_t m_nFilters; // number of mel filters to use
    size_t m_nCoeffs; // number of MFCC coefficients to return (default = 13)
    bool m_useEnergy; // whether to replace coeff0 with log of the true energy of the frame (true) or not (false)
    float m_lifterExp;  // the exponent to use for liftering (0 = no liftering, 1 = max liftering)

    vector< vector<float> > m_filterbank;
};


#endif // _MFCC_H_
