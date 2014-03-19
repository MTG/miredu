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

    See RMS.h for description
*/

#include "RMS.h"


RMS::RMS(float inputSampleRate) :
    Plugin(inputSampleRate),
    // Also be sure to set your plugin parameters (presumably stored
    // in member variables) to their default values here -- the host
    // will not do that for you
	m_blockSize(0),
	m_stepSize(0)
{
}

RMS::~RMS()
{
}

string
RMS::getIdentifier() const
{
    return "rms";
}

string
RMS::getName() const
{
    return "MIR.EDU: RMS";
}

string
RMS::getDescription() const
{
    // Return something helpful here!
    return "Compute the root mean square of the signal for each frame";
}

string
RMS::getMaker() const
{
    // Your name here
    return "MIR.EDU by Justin Salamon";
}

int
RMS::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
RMS::getCopyright() const
{
    // This function is not ideally named.  It does not necessarily
    // need to say who made the plugin -- getMaker does that -- but it
    // should indicate the terms under which it is distributed.  For
    // example, "Copyright (year). All Rights Reserved", or "GPL"
    return "GPL";
}

RMS::InputDomain
RMS::getInputDomain() const
{
    return TimeDomain;
}

size_t
RMS::getPreferredBlockSize() const
{
    return 0; // 0 means "I can handle any block size"
}

size_t 
RMS::getPreferredStepSize() const
{
    return 0; // 0 means "anything sensible"; in practice this
              // means the same as the block size for TimeDomain
              // plugins, or half of it for FrequencyDomain plugins
}

size_t
RMS::getMinChannelCount() const
{
    return 1;
}

size_t
RMS::getMaxChannelCount() const
{
    return 1;
}

RMS::ParameterList
RMS::getParameterDescriptors() const
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
RMS::getParameter(string identifier) const
{
   // if (identifier == "parameter") {
   //     return 5; // return the ACTUAL current value of your parameter here!
   // }
    return 0;
}

void
RMS::setParameter(string identifier, float value) 
{
    //if (identifier == "parameter") {
    //    // set the actual value of your parameter
    //}
}

RMS::ProgramList
RMS::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
RMS::getCurrentProgram() const
{
    return ""; // no programs
}

void
RMS::selectProgram(string name)
{
}

RMS::OutputList
RMS::getOutputDescriptors() const
{
    OutputList list;

    // See OutputDescriptor documentation for the possibilities here.
    // Every plugin must have at least one output.

    OutputDescriptor d;
    d.identifier = "rms";
    d.name = "RMS";
    d.description = "";
    d.unit = "";
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
RMS::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
	channels > getMaxChannelCount()) return false;

    // Real initialisation work goes here!
	m_blockSize = blockSize;
	m_stepSize = stepSize;

    return true;
}

void
RMS::reset()
{
    // Clear buffers, reset stored values, etc
}

RMS::FeatureSet
RMS::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    // Do actual work!
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

	Feature f;
    f.hasTimestamp = false;
    f.values.push_back(rms);

    FeatureSet fs;
    fs[0].push_back(f);

	return fs;
}

RMS::FeatureSet
RMS::getRemainingFeatures()
{
    return FeatureSet();
}

