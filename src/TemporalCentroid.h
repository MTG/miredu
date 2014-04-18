/**
 * @file TemporalCentroid.h
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
 * Feature name: Temporal Centroid <br/>
 * Description: Compute the temporal centroid, which is the centre of gravity
 * of the signal energy. <br/>
 * NOTE: we use the RMS to represent the signal energy. <br/>
 * Unit: time (seconds) <br/>
 * Formula: temporalcentroid = sum (t_i * e_i) / sum (e_i) <br/>
 * where t_i is the time at frame i, and e_i is the energy at frame i. <br/>
 * NOTE 2: we only compute the temporal centroid between times n1 and n2,
 * which are the first and last times the signal RMS is above 15% of its
 * maximum value. <br/>
 * NOTE 3: the temporal centroid is returned as a timestamp with no corresponding value. <br/>
 * Reference: G. Peeters, B. L. Giordano, P. Susini, N. Misdariis, and S. McAdams. The timbre toolbox: Extracting audio
 * descriptors from musical signals. J. Acoust. Soc. Am., 130(5):2902–2916, 2011. <br/>
 * PDF: http://mt.music.mcgill.ca/mpcl/publications/peeters-giordano-susini-misdariis-mcadams-2011 <br/>
 */


// Remember to use a different guard symbol in each header!
#ifndef _TEMPORALCENTROID_H_
#define _TEMPORALCENTROID_H_

#include <vector>
#include <cmath>
#include <algorithm>
#include <ostream>
#include <vamp-sdk/Plugin.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;

class TemporalCentroid : public Vamp::Plugin
{
public:
    TemporalCentroid(float inputSampleRate);
    virtual ~TemporalCentroid();

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

    vector<float> m_rms;
    vector<Vamp::RealTime> m_timestamps;
};



#endif // _TEMPORALCENTROID_H_
