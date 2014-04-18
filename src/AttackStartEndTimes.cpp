/**
 * @file AttackStartEndTimes.cpp
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
 * See AttackStartEndTimes.h for description
*/

#include "AttackStartEndTimes.h"


AttackStartEndTimes::AttackStartEndTimes(float inputSampleRate) :
    Plugin(inputSampleRate),
    // Also be sure to set your plugin parameters (presumably stored
    // in member variables) to their default values here -- the host
    // will not do that for you
    m_blockSize(0),
    m_stepSize(0)
{
}

AttackStartEndTimes::~AttackStartEndTimes()
{
}

string
AttackStartEndTimes::getIdentifier() const
{
    return "attackstartendtimes";
}

string
AttackStartEndTimes::getName() const
{
    return "MIR.EDU: Attack Start/End Times";
}

string 
AttackStartEndTimes::getDescription() const 
{     
    // Return something helpful here!
    return "Compute the start and end times of the attack of the signal. The start and end times of the attack are "
    "computed according to Peeters (2004), see code/documentation for further details. NOTE: the accuracy of the "
    "estimation depends on the step (hop) size of the analysis, the smaller the better. The times are returned as "
    "timestamps without any values.";
}

string
AttackStartEndTimes::getMaker() const
{
    // Your name here
    return "MIR.EDU by Justin Salamon";
}

int
AttackStartEndTimes::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
AttackStartEndTimes::getCopyright() const
{
    // This function is not ideally named.  It does not necessarily
    // need to say who made the plugin -- getMaker does that -- but it
    // should indicate the terms under which it is distributed.  For
    // example, "Copyright (year). All Rights Reserved", or "GPL"
    return "GPL";
}

AttackStartEndTimes::InputDomain
AttackStartEndTimes::getInputDomain() const
{
    return TimeDomain;
}

size_t
AttackStartEndTimes::getPreferredBlockSize() const
{
    return 0; // 0 means "I can handle any block size"
}

size_t 
AttackStartEndTimes::getPreferredStepSize() const
{
    // We need a small hop size to get an accurate estimate of the attack time
    // 64 = 1.5ms assuming a sampling rate of = 44100.
    return 64; // 0 means "anything sensible"; in practice this
              // means the same as the block size for TimeDomain
              // plugins, or half of it for FrequencyDomain plugins
}

size_t
AttackStartEndTimes::getMinChannelCount() const
{
    return 1;
}

size_t
AttackStartEndTimes::getMaxChannelCount() const
{
    return 1;
}

AttackStartEndTimes::ParameterList
AttackStartEndTimes::getParameterDescriptors() const
{
    ParameterList list;

    // If the plugin has no adjustable parameters, return an empty
    // list here (and there's no need to provide implementations of
    // getParameter and setParameter in that case either).

    // Note that it is your responsibility to make sure the parameters
    // start off having their default values (e.g. in the constructor
    // above).  The host needs to know the default value so it can do
    // things like provide a "reset to default" function, but it will
    // not explicitly set your parameters to their defaults for you if
    // they have not changed in the mean time.

    /* No parameters
    ParameterDescriptor d;
    d.identifier = "parameter";
    d.name = "Some Parameter";
    d.description = "";
    d.unit = "";
    d.minValue = 0;
    d.maxValue = 10;
    d.defaultValue = 5;
    d.isQuantized = false;
    list.push_back(d);
    */

    return list;
}

float
AttackStartEndTimes::getParameter(string identifier) const
{
   // if (identifier == "parameter") {
   //     return 5; // return the ACTUAL current value of your parameter here!
   // }
    return 0;
}

void
AttackStartEndTimes::setParameter(string identifier, float value) 
{
    //if (identifier == "parameter") {
    //    // set the actual value of your parameter
    //}
}

AttackStartEndTimes::ProgramList
AttackStartEndTimes::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
AttackStartEndTimes::getCurrentProgram() const
{
    return ""; // no programs
}

void
AttackStartEndTimes::selectProgram(string name)
{
}

AttackStartEndTimes::OutputList
AttackStartEndTimes::getOutputDescriptors() const
{
    OutputList list;

    // See OutputDescriptor documentation for the possibilities here.
    // Every plugin must have at least one output.

    OutputDescriptor d;
    d.identifier = "attackstartendtimes";
    d.name = "Attack Start/End Times";
    d.description = "";
    d.unit = "";
    d.hasFixedBinCount = true;
    d.binCount = 0;
    d.hasKnownExtents = false;
    d.isQuantized = false;
    d.sampleType = OutputDescriptor::VariableSampleRate;
    d.sampleRate = 0;
    d.hasDuration = false;
    list.push_back(d);

    return list;
}

bool
AttackStartEndTimes::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
    channels > getMaxChannelCount()) return false;

    // Real initialisation work goes here!
    m_blockSize = blockSize;
    m_stepSize = stepSize;

    m_rms.clear();
    m_timestamps.clear();

    return true;
}

void
AttackStartEndTimes::reset()
{
    // Clear buffers, reset stored values, etc
    m_rms.clear();
    m_timestamps.clear();
}

AttackStartEndTimes::FeatureSet
AttackStartEndTimes::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    // First we need to compute the RMS
    float energy = 0.0f;

    size_t i = 0; // note: same type as m_blockSize

    while (i < m_blockSize)
    {
        float sample = inputBuffers[0][i];
        energy += sample * sample;
        ++i;
    }

    float mean_energy = energy / m_blockSize;
    float rms = sqrt(mean_energy);

    m_rms.push_back(rms);
    m_timestamps.push_back(timestamp);

    return FeatureSet();
}

AttackStartEndTimes::FeatureSet
AttackStartEndTimes::getRemainingFeatures()
{
    Vamp::RealTime start_time, end_time;
    if (m_rms.empty()) // empty signal
    {
        start_time = Vamp::RealTime::fromSeconds(0.0);
        end_time = Vamp::RealTime::fromSeconds(0.0);
    }
    else
    {
        // Find the start and end times of the attack
        float max_rms = *(max_element(m_rms.begin(),m_rms.end())); // find the maximum RMS value

        float thresholds[] = {0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f,0.8f,0.9f,1.0f}; // thresholds: fractions of max rms
        float threshold_times[10]; // in seconds

        int rms_index = 0;
        
        // Find the times when the rms reaches each of the 10 thresholds
        for (int i=0; i<10; i++)
        {
            float threshold = thresholds[i] * max_rms;
            while (m_rms[rms_index] < threshold)
                rms_index++;
            threshold_times[i] = m_timestamps[rms_index].sec + (float)m_timestamps[rms_index].nsec/1e9;
        }

        /* debugging
        for (int i=0; i<10; i++)
            cout << threshold_times[i] << " ";
        cout << endl;
        */

        float efforts[9];
        float effort_mean = 0.0f;
        for (int i=0; i<9; i++)
        {
            efforts[i] = threshold_times[i+1] - threshold_times[i];
            effort_mean += efforts[i];
        }

        effort_mean /= 9.0f;
        
        int alpha = 3; // impirical parameter, see reference for details
        float t_start, t_end;
        int t_index = 0;
        while (efforts[t_index] >= alpha * effort_mean)
            t_index++;
        t_start = threshold_times[t_index]; // attack start is the beginning of the effort

        t_index = 8;
        while (efforts[t_index] >= alpha * effort_mean)
            t_index--;
        t_end = threshold_times[t_index+1]; // attack end is the end of the effort

        // cout << "t_start: " << t_start << " t_end: " << t_end << endl; // debugging

        start_time = Vamp::RealTime::fromSeconds(t_start);
        end_time = Vamp::RealTime::fromSeconds(t_end);
    }
    
    FeatureSet fs;
    Feature f;
    
    f.hasTimestamp = true;
    f.timestamp = start_time;
    fs[0].push_back(f);

    f.timestamp = end_time;
    fs[0].push_back(f);

    return fs;
    return FeatureSet();
}

