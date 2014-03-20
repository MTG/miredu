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

    Feature name: Spectral Flux
    Description: Compute the spectral flux between evey 2 consecutive frames of the signal.
    The flux is measured between the current frame and the previous one, so fo the first 
    frame the flux is always zero.
    The spectral flux, defined as the difference between the spectral magnitude
    of every 2 consecutive frames, can be calculated in various ways. This implementation
    follows the one proposed in the reference mentioned below, i.e. the difference is 
    defined as 1 minus the normalised correlation between successive magnitude spectra.
    NOTE: we do NOT use the L2-norm (Euclidian distance)
    NOTE 2: the spectral magnitudes are not normalised
    NOTE 3: if both frames are silent flux = 0, if only one is silent flux = 1.
    Reference: http://asadl.org/jasa/resource/1/jasman/v130/i5/p2902_s1/cart.do
    PDF: http://mt.music.mcgill.ca/mpcl/publications/peeters-giordano-susini-misdariis-mcadams-2011
 */

// Remember to use a different guard symbol in each header!
#ifndef _SPECTRALFlux_H_
#define _SPECTRALFlux_H_

#include <vector>
#include <cmath>
#include <algorithm>
#include <ostream>
#include <vamp-sdk/Plugin.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;

class SpectralFlux : public Vamp::Plugin
{
public:
    SpectralFlux(float inputSampleRate);
    virtual ~SpectralFlux();

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
    // plugin-specific data and methods go here
    size_t m_blockSize;
    size_t m_stepSize;
    vector<float> m_prev_mags;
    float m_sum_prev_sqr;
};


#endif // _SPECTRALFlux_H_
