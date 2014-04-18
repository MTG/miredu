/**
 * @file SpectralRolloff.cpp
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
 * See SpectralRolloff.h for description
*/

#include "SpectralRolloff.h"

SpectralRolloff::SpectralRolloff(float inputSampleRate) :
    Plugin(inputSampleRate),
    // Also be sure to set your plugin parameters (presumably stored
    // in member variables) to their default values here -- the host
    // will not do that for you
    m_blockSize(0),
    m_stepSize(0),
    m_rolloffthreshold(95)
{
}

SpectralRolloff::~SpectralRolloff()
{
}

string
SpectralRolloff::getIdentifier() const
{
    return "spectralrolloff";
}

string
SpectralRolloff::getName() const
{
    return "MIR.EDU: Spectral Roll-off";
}

string
SpectralRolloff::getDescription() const
{
    // Return something helpful here!
    return "Compute the spectral roll-off of the signal for each frame, defined as the frequency below which 95% of "
    "the signal energy is contained. The threshold (95%) is defined as a paramtere that can be changed by the user. "
    "The unit of the values returned is Hz. If the frame is completely silent a value of 0 is returned.";
}

string
SpectralRolloff::getMaker() const
{
    // Your name here
    return "MIR.EDU by Justin Salamon";
}

int
SpectralRolloff::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
SpectralRolloff::getCopyright() const
{
    // This function is not ideally named.  It does not necessarily
    // need to say who made the plugin -- getMaker does that -- but it
    // should indicate the terms under which it is distributed.  For
    // example, "Copyright (year). All Rights Reserved", or "GPL"
    return "GPL";
}

SpectralRolloff::InputDomain
SpectralRolloff::getInputDomain() const
{
    return FrequencyDomain;
}

size_t
SpectralRolloff::getPreferredBlockSize() const
{
    return 0; // 0 means "I can handle any block size"
}

size_t 
SpectralRolloff::getPreferredStepSize() const
{
    return 0; // 0 means "anything sensible"; in practice this
              // means the same as the block size for TimeDomain
              // plugins, or half of it for FrequencyDomain plugins
}

size_t
SpectralRolloff::getMinChannelCount() const
{
    return 1;
}

size_t
SpectralRolloff::getMaxChannelCount() const
{
    return 1;
}

SpectralRolloff::ParameterList
SpectralRolloff::getParameterDescriptors() const
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

    ParameterDescriptor d;
    d.identifier = "rolloffthreshold";
    d.name = "Roll-off threshold";
    d.description = "Threshold (percentage) for determining the roll-off frequency.";
    d.unit = "";
    d.minValue = 0;
    d.maxValue = 100;
    d.defaultValue = 95;
    d.isQuantized = true;
    d.quantizeStep = 1;
    list.push_back(d);

    return list;
}

float
SpectralRolloff::getParameter(string identifier) const
{
    if (identifier == "rolloffthreshold") 
        return m_rolloffthreshold;
    
    return 0;
}

void
SpectralRolloff::setParameter(string identifier, float value) 
{
    if (identifier == "rolloffthreshold")
    {
        if (value > 100)
            m_rolloffthreshold = 100;
        else if (value < 0)
            m_rolloffthreshold = 0;
        else
            m_rolloffthreshold = value;
    }
}

SpectralRolloff::ProgramList
SpectralRolloff::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
SpectralRolloff::getCurrentProgram() const
{
    return ""; // no programs
}

void
SpectralRolloff::selectProgram(string name)
{
}

SpectralRolloff::OutputList
SpectralRolloff::getOutputDescriptors() const
{
    OutputList list;

    // See OutputDescriptor documentation for the possibilities here.
    // Every plugin must have at least one output.

    OutputDescriptor d;
    d.identifier = "spectralrolloff";
    d.name = "Spectral Rolloff";
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
SpectralRolloff::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
    channels > getMaxChannelCount()) return false;

    // Real initialisation work goes here!
    m_blockSize = blockSize;
    m_stepSize = stepSize;

    return true;
}

void
SpectralRolloff::reset()
{
    // Clear buffers, reset stored values, etc
}

SpectralRolloff::FeatureSet
SpectralRolloff::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    // Do actual work!
    float magnitude_sqr_sum = 0.0f;
    vector<float> magnitudes_sqr(m_blockSize/2);
    vector<float> frequencies(m_blockSize/2);

    for (size_t i=0; i<m_blockSize; i+=2)
    {
        float breal = inputBuffers[0][i];
        float bimag = inputBuffers[0][i+1];
        float magnitude = sqrt(breal*breal + bimag*bimag) / (m_blockSize/4);
        float frequency = (i/2) * (float)m_inputSampleRate / (float)m_blockSize;
        magnitude_sqr_sum += magnitude * magnitude;
        magnitudes_sqr[i/2] = magnitude * magnitude;
        frequencies[i/2] = frequency;
    }

    float spectralrolloff;

    if (magnitude_sqr_sum == 0)
        spectralrolloff = 0;
    else
    {
        float threshold = m_rolloffthreshold / 100.0f; // convert from percentage to fraction
        float rolloff_sum = 0.0f;
        int rolloff_index = 0;
        while (rolloff_index < (int)(m_blockSize/2) && rolloff_sum < threshold * magnitude_sqr_sum)
        {
            rolloff_sum += magnitudes_sqr[rolloff_index];
            rolloff_index++;
        }
        spectralrolloff = frequencies[rolloff_index-1]; // -1 because rolloff_index is increased after the summation
    }

    Feature f;
    f.hasTimestamp = false;
    f.values.push_back(spectralrolloff);

    FeatureSet fs;
    fs[0].push_back(f);

    return fs;
}

SpectralRolloff::FeatureSet
SpectralRolloff::getRemainingFeatures()
{
    return FeatureSet();
}

