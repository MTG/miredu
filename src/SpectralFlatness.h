/*
    MIR.EDU

    A vamp plug-in library for teaching MIR.
    Copyright (C) 2013 Justin Salamon.

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
 
    Feature name: Spectral Flatness
    Description: Compute the spectral flatness of each frame of the signal.
    The flatness is defined as the ratio of the geometric and arithmetical 
    means of the spectral magnitudes.
    Unit: none
    Formula: flatness = (prod(m_i)^(1/K)) / (sum(m_i)*(1/K))
    where i = 1..K
    NOTE: for the computation of the geometrical mean we actually
    compute the sum of logarithms rather than the product, since
    otherwise we run into arithmetic underflow. The relationship
    between the geometric mean and the arithmetic mean of logarithms
    is described here: http://en.wikipedia.org/wiki/Geometric_mean#Relationship_with_arithmetic_mean_of_logarithms
    NOTE 2: DC component not used.
    Reference:
 */

// Remember to use a different guard symbol in each header!
#ifndef _SPECTRALFLATNESS_H_
#define _SPECTRALFLATNESS_H_

#include <vector>
#include <cmath>
#include <ostream>
#include <vamp-sdk/Plugin.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;

class SpectralFlatness : public Vamp::Plugin
{
public:
    SpectralFlatness(float inputSampleRate);
    virtual ~SpectralFlatness();

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
};


#endif // _SPECTRALFLATNESS_H_
