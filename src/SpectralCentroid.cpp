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

    See SpectralCentroid.h for description
*/

#include "SpectralCentroid.h"

SpectralCentroid::SpectralCentroid(float inputSampleRate) :
    Plugin(inputSampleRate),
    // Also be sure to set your plugin parameters (presumably stored
    // in member variables) to their default values here -- the host
    // will not do that for you
	m_blockSize(0),
	m_stepSize(0)
{
}

SpectralCentroid::~SpectralCentroid()
{
}

string
SpectralCentroid::getIdentifier() const
{
    return "spectralcentroid";
}

string
SpectralCentroid::getName() const
{
    return "MIR.EDU: Spectral Centroid";
}

string
SpectralCentroid::getDescription() const
{
    // Return something helpful here!
    return "Compute the spectral centroid of the signal for each frame. The unit of the values returned is Hz. If the frame is completely silent a value of 0 is returned.";
}

string
SpectralCentroid::getMaker() const
{
    // Your name here
    return "MIR.EDU by Justin Salamon";
}

int
SpectralCentroid::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
SpectralCentroid::getCopyright() const
{
    // This function is not ideally named.  It does not necessarily
    // need to say who made the plugin -- getMaker does that -- but it
    // should indicate the terms under which it is distributed.  For
    // example, "Copyright (year). All Rights Reserved", or "GPL"
    return "GPL";
}

SpectralCentroid::InputDomain
SpectralCentroid::getInputDomain() const
{
	return FrequencyDomain;
}

size_t
SpectralCentroid::getPreferredBlockSize() const
{
    return 0; // 0 means "I can handle any block size"
}

size_t 
SpectralCentroid::getPreferredStepSize() const
{
    return 0; // 0 means "anything sensible"; in practice this
              // means the same as the block size for TimeDomain
              // plugins, or half of it for FrequencyDomain plugins
}

size_t
SpectralCentroid::getMinChannelCount() const
{
    return 1;
}

size_t
SpectralCentroid::getMaxChannelCount() const
{
    return 1;
}

SpectralCentroid::ParameterList
SpectralCentroid::getParameterDescriptors() const
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
SpectralCentroid::getParameter(string identifier) const
{
   // if (identifier == "parameter") {
   //     return 5; // return the ACTUAL current value of your parameter here!
   // }
    return 0;
}

void
SpectralCentroid::setParameter(string identifier, float value) 
{
    //if (identifier == "parameter") {
    //    // set the actual value of your parameter
    //}
}

SpectralCentroid::ProgramList
SpectralCentroid::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
SpectralCentroid::getCurrentProgram() const
{
    return ""; // no programs
}

void
SpectralCentroid::selectProgram(string name)
{
}

SpectralCentroid::OutputList
SpectralCentroid::getOutputDescriptors() const
{
    OutputList list;

    // See OutputDescriptor documentation for the possibilities here.
    // Every plugin must have at least one output.

    OutputDescriptor d;
    d.identifier = "spectralcentroid";
    d.name = "Spectral Centroid";
    d.description = "";
    d.unit = "Hz";
    d.hasFixedBinCount = true;
    d.binCount = 1;
    d.hasKnownExtents = false;
    d.isQuantized = false;
    d.sampleType = OutputDescriptor::OneSamplePerStep;
    d.hasDuration = false;
    list.push_back(d);

    return list;
}

bool
SpectralCentroid::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
	channels > getMaxChannelCount()) return false;

    // Real initialisation work goes here!
	m_blockSize = blockSize;
	m_stepSize = stepSize;

    return true;
}

void
SpectralCentroid::reset()
{
    // Clear buffers, reset stored values, etc
}

SpectralCentroid::FeatureSet
SpectralCentroid::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    // Do actual work!
	float magnitude_sum = 0.0f;
	float weighted_frequency_sum = 0.0f;

    for (size_t i=0; i<m_blockSize; i+=2)
	{
		float breal = inputBuffers[0][i];
		float bimag = inputBuffers[0][i+1];
        float magnitude = sqrt(breal*breal + bimag*bimag) / (m_blockSize/4); // normalise by window size, ideally by 0.5*sum(window), this is an approximation that works best if a hann window is used.
		float frequency = (i/2) * (float)m_inputSampleRate / (float)m_blockSize;
        magnitude_sum += magnitude;
		weighted_frequency_sum += frequency * magnitude;
    }

	float spectralcentroid;
	if (magnitude_sum == 0)
		spectralcentroid = 0;
	else
		spectralcentroid = weighted_frequency_sum / magnitude_sum;

	Feature f;
    f.hasTimestamp = false;
    f.values.push_back(spectralcentroid);

    FeatureSet fs;
    fs[0].push_back(f);

	return fs;
}

SpectralCentroid::FeatureSet
SpectralCentroid::getRemainingFeatures()
{
    return FeatureSet();
}

