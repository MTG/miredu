/**
 * @file LogAttackTime.h
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
 * Feature name: Log Attack Time <br/>
 * Description: Compute the logarithm (base 10) of the duration of the attack of the signal. <br/>
 * Unit: log10(seconds) <br/>
 * Formula: LogAttackTime = log10(t_end - t_start) <br/>
 * where t_start and t_end are the estimated start and end times (in seconds) of the attack <br/>
 * of the signal, which are estimated as explained in the reference. <br/>
 * NOTE: the timestamp of the returned value represents the start time of the attack. <br/>
 * NOTE 2: to visualise the attack start and end times use the 'Attack Start/End Times' pluging <br/>
 * NOTE 3: the accuracy of the estimation depends on the step (hop) size of the analysis, the <br/>
 * smaller the better. <br/>
 * Reference: G. Peeters, B. L. Giordano, P. Susini, N. Misdariis, and S. McAdams. The timbre toolbox: Extracting audio
 * descriptors from musical signals. J. Acoust. Soc. Am., 130(5):2902–2916, 2011. </br>
 * PDF: http://mt.music.mcgill.ca/mpcl/publications/peeters-giordano-susini-misdariis-mcadams-2011 <br/>
 */


// Remember to use a different guard symbol in each header!
#ifndef _LOGATTACKTIME_H_
#define _LOGATTACKTIME_H_

#include <vector>
#include <algorithm>
#include <cmath>
#include <ostream>
#include <vamp-sdk/Plugin.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;

class LogAttackTime : public Vamp::Plugin
{
public:
    LogAttackTime(float inputSampleRate);
    virtual ~LogAttackTime();

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



#endif // _LOGATTACKTIME_H_
