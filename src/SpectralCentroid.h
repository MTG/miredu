/**
 * @file SpectralCentroid.h
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
 * Feature name: Spectral Centroid </br>
 * Description: Compute the spectral centroid of each frame of the signal. </br>
 * If a frame is completely silent, a value of 0 is returned. </br>
 * Unit: Hz </br>
 * Formula: </br>
 * Reference1: http://asadl.org/jasa/resource/1/jasman/v130/i5/p2902_s1/cart.do <br/>
 * Reference2: http://en.wikipedia.org/wiki/Spectral_centroid </br>
 * PDF: http://mt.music.mcgill.ca/mpcl/publications/peeters-giordano-susini-misdariis-mcadams-2011 <br/>
 */


// Remember to use a different guard symbol in each header!
#ifndef _SPECTRALCENTROID_H_
#define _SPECTRALCENTROID_H_

#include <cmath>
#include <ostream>
#include <vamp-sdk/Plugin.h>

using std::string;
using std::cout;
using std::endl;

class SpectralCentroid : public Vamp::Plugin
{
public:
    SpectralCentroid(float inputSampleRate);
    virtual ~SpectralCentroid();

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


#endif // _SPECTRALCENTROID_H_
