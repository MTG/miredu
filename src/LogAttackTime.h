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
 
    Feature name: Log Attack Time
    Description: Compute the logarithm (base 10) of the duration of the attack of the signal.
    Unit: log10(seconds)
    Formula: LogAttackTime = log10(t_end - t_start)
    where t_start and t_end are the estimated start and end times (in seconds) of the attack
    of the signal, which are estimated as explained in the reference.
    NOTE: the timestamp of the returned value represents the start time of the attack.
    NOTE 2: to visualise the attack start and end times use the 'Attack Start/End Times' pluging
    NOTE 3: the accuracy of the estimation depends on the step (hop) size of the analysis, the
    smaller the better.
    Reference: http://asadl.org/jasa/resource/1/jasman/v130/i5/p2902_s1/cart.do
    PDF: http://mt.music.mcgill.ca/mpcl/publications/peeters-giordano-susini-misdariis-mcadams-2011
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
