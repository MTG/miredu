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

    See TemporalCentroid.h for description
*/

#include "TemporalCentroid.h"


TemporalCentroid::TemporalCentroid(float inputSampleRate) :
    Plugin(inputSampleRate),
    // Also be sure to set your plugin parameters (presumably stored
    // in member variables) to their default values here -- the host
    // will not do that for you
	m_blockSize(0),
	m_stepSize(0)
{
}

TemporalCentroid::~TemporalCentroid()
{
}

string
TemporalCentroid::getIdentifier() const
{
    return "temporalcentroid";
}

string
TemporalCentroid::getName() const
{
    return "MIR.EDU: Temporal Centroid";
}

string
TemporalCentroid::getDescription() const
{
    // Return something helpful here!
    return "Compute the temporal centroid of the entire signal, which is the centre of gravity of the energy of the signal. Energy is represented by the RMS of the signal. The termporal centroid is computed between times n1 and n2 which are the first and last times the signal RMS is above 15% of its maximum value. The temporal centroid is returned as a timestamp with no corresponding value.";
}

string
TemporalCentroid::getMaker() const
{
    // Your name here
    return "MIR.EDU by Justin Salamon";
}

int
TemporalCentroid::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
TemporalCentroid::getCopyright() const
{
    // This function is not ideally named.  It does not necessarily
    // need to say who made the plugin -- getMaker does that -- but it
    // should indicate the teTemporalCentroid under which it is distributed.  For
    // example, "Copyright (year). All Rights Reserved", or "GPL"
    return "GPL";
}

TemporalCentroid::InputDomain
TemporalCentroid::getInputDomain() const
{
    return TimeDomain;
}

size_t
TemporalCentroid::getPreferredBlockSize() const
{
    return 0; // 0 means "I can handle any block size"
}

size_t 
TemporalCentroid::getPreferredStepSize() const
{
    return 0; // 0 means "anything sensible"; in practice this
              // means the same as the block size for TimeDomain
              // plugins, or half of it for FrequencyDomain plugins
}

size_t
TemporalCentroid::getMinChannelCount() const
{
    return 1;
}

size_t
TemporalCentroid::getMaxChannelCount() const
{
    return 1;
}

TemporalCentroid::ParameterList
TemporalCentroid::getParameterDescriptors() const
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
TemporalCentroid::getParameter(string identifier) const
{
   // if (identifier == "parameter") {
   //     return 5; // return the ACTUAL current value of your parameter here!
   // }
    return 0;
}

void
TemporalCentroid::setParameter(string identifier, float value) 
{
    //if (identifier == "parameter") {
    //    // set the actual value of your parameter
    //}
}

TemporalCentroid::ProgramList
TemporalCentroid::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
TemporalCentroid::getCurrentProgram() const
{
    return ""; // no programs
}

void
TemporalCentroid::selectProgram(string name)
{
}

TemporalCentroid::OutputList
TemporalCentroid::getOutputDescriptors() const
{
    OutputList list;

    // See OutputDescriptor documentation for the possibilities here.
    // Every plugin must have at least one output.

    OutputDescriptor d;
    d.identifier = "temporalcentroid";
    d.name = "Temporal Centroid";
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
TemporalCentroid::initialise(size_t channels, size_t stepSize, size_t blockSize)
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
TemporalCentroid::reset()
{
    // Clear buffers, reset stored values, etc
	m_rms.clear();
	m_timestamps.clear();
}

TemporalCentroid::FeatureSet
TemporalCentroid::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
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

	// cout << timestamp+Vamp::RealTime::fromSeconds(1) << endl; // debugging
	//cout << (float)timestamp.nsec/1e9 << endl;
	//cout << timestamp << ": " << timestamp.sec + (float)timestamp.nsec/1e9 << endl; // CONVERSION!

	return FeatureSet();
}

TemporalCentroid::FeatureSet
TemporalCentroid::getRemainingFeatures()
{
	Vamp::RealTime centroid_timestamp;

	if (m_rms.empty()) // empty signal
		centroid_timestamp = Vamp::RealTime::fromSeconds(0);
	else
	{
		// Compute the temporal centroid
		float max_rms = *(max_element(m_rms.begin(),m_rms.end())); // find the maximum RMS value
		float rms_threshold = 0.15f * max_rms; // threshold for starting to compute temporal centroid, 15% of max RMS

		int n1 = 0;
		while (m_rms[n1] < rms_threshold) // find first frame where RMS > 15% of max RMS
			n1++;

		int n2 = (int)m_rms.size() - 1;
		while (m_rms[n2] < rms_threshold)
			n2--;

		float sum_weighted_time = 0.0f;
		float sum_rms = 0.0f;
		for (int i=n1; i<=n2; i++)
		{
			sum_rms += m_rms[i];
			sum_weighted_time += m_rms[i] * (m_timestamps[i].sec + (float)m_timestamps[i].nsec/1e9);
		}

		float temporalcentroid = sum_weighted_time / sum_rms;
		centroid_timestamp = Vamp::RealTime::fromSeconds(temporalcentroid);
	}

	Feature f;
    f.hasTimestamp = true;
	f.timestamp = centroid_timestamp;

    FeatureSet fs;
    fs[0].push_back(f);

	return fs;
    return FeatureSet();
}

