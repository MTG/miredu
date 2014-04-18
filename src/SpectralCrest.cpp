/**
 * @file SpectralCrest.cpp
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
 * See SpectralCrest.h for description
*/

#include "SpectralCrest.h"

SpectralCrest::SpectralCrest(float inputSampleRate) :
    Plugin(inputSampleRate),
    // Also be sure to set your plugin parameters (presumably stored
    // in member variables) to their default values here -- the host
    // will not do that for you
    m_blockSize(0),
    m_stepSize(0)
{
}

SpectralCrest::~SpectralCrest()
{
}

string
SpectralCrest::getIdentifier() const
{
    return "spectralcrest";
}

string
SpectralCrest::getName() const
{
    return "MIR.EDU: Spectral Crest";
}

string
SpectralCrest::getDescription() const
{
    // Return something helpful here!
    return "Compute the spectral crest of the signal for each frame. The crest is defined as the ratio between the "
    "maximum spectral magnitude in the frame and the arithmetical mean of the spectral magnitudes. The minimum "
    "possible value is 1 (flat spectrum) and it increases the peakier the spectrum is. If the frame is completely "
    "silent a value of 1 (flat spectrum) is returned.";
}

string
SpectralCrest::getMaker() const
{
    // Your name here
    return "MIR.EDU by Justin Salamon";
}

int
SpectralCrest::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
SpectralCrest::getCopyright() const
{
    // This function is not ideally named.  It does not necessarily
    // need to say who made the plugin -- getMaker does that -- but it
    // should indicate the terms under which it is distributed.  For
    // example, "Copyright (year). All Rights Reserved", or "GPL"
    return "GPL";
}

SpectralCrest::InputDomain
SpectralCrest::getInputDomain() const
{
    return FrequencyDomain;
}

size_t
SpectralCrest::getPreferredBlockSize() const
{
    return 0; // 0 means "I can handle any block size"
}

size_t 
SpectralCrest::getPreferredStepSize() const
{
    return 0; // 0 means "anything sensible"; in practice this
              // means the same as the block size for TimeDomain
              // plugins, or half of it for FrequencyDomain plugins
}

size_t
SpectralCrest::getMinChannelCount() const
{
    return 1;
}

size_t
SpectralCrest::getMaxChannelCount() const
{
    return 1;
}

SpectralCrest::ParameterList
SpectralCrest::getParameterDescriptors() const
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
SpectralCrest::getParameter(string identifier) const
{
   // if (identifier == "parameter") {
   //     return 5; // return the ACTUAL current value of your parameter here!
   // }
    return 0;
}

void
SpectralCrest::setParameter(string identifier, float value) 
{
    //if (identifier == "parameter") {
    //    // set the actual value of your parameter
    //}
}

SpectralCrest::ProgramList
SpectralCrest::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
SpectralCrest::getCurrentProgram() const
{
    return ""; // no programs
}

void
SpectralCrest::selectProgram(string name)
{
}

SpectralCrest::OutputList
SpectralCrest::getOutputDescriptors() const
{
    OutputList list;

    // See OutputDescriptor documentation for the possibilities here.
    // Every plugin must have at least one output.

    OutputDescriptor d;
    d.identifier = "spectralcrest";
    d.name = "Spectral Crest";
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
SpectralCrest::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
    channels > getMaxChannelCount()) return false;

    // Real initialisation work goes here!
    m_blockSize = blockSize;
    m_stepSize = stepSize;

    return true;
}

void
SpectralCrest::reset()
{
    // Clear buffers, reset stored values, etc
}

SpectralCrest::FeatureSet
SpectralCrest::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    // Without magnitude normalization
    float mag_sum = 0.0f;
    float max_mag = 0.0f;

    for (size_t i=0; i<m_blockSize; i+=2)
    {
        float breal = inputBuffers[0][i];
        float bimag = inputBuffers[0][i+1];
        float mag = sqrt(breal*breal + bimag*bimag) / (m_blockSize/4);
        mag_sum += mag;
        max_mag = (mag > max_mag) ? mag : max_mag; // check if we have a new maximum
    }

    float spectralCrest;

    if (mag_sum == 0)
        spectralCrest = 1;
    else
    {
        float arithmetical_mean = mag_sum / float(m_blockSize/2);
        spectralCrest =  max_mag / arithmetical_mean;
    }

    Feature f;
    f.hasTimestamp = false;
    f.values.push_back(spectralCrest);

    FeatureSet fs;
    fs[0].push_back(f);

    return fs;
}

SpectralCrest::FeatureSet
SpectralCrest::getRemainingFeatures()
{
    return FeatureSet();
}

