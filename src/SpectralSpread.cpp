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

    See SpectralSpread.h for description
*/

#include "SpectralSpread.h"

SpectralSpread::SpectralSpread(float inputSampleRate) :
    Plugin(inputSampleRate),
    // Also be sure to set your plugin parameters (presumably stored
    // in member variables) to their default values here -- the host
    // will not do that for you
    m_blockSize(0),
    m_stepSize(0)
{
}

SpectralSpread::~SpectralSpread()
{
}

string
SpectralSpread::getIdentifier() const
{
    return "spectralspread";
}

string
SpectralSpread::getName() const
{
    return "MIR.EDU: Spectral Spread";
}

string
SpectralSpread::getDescription() const
{
    // Return something helpful here!
    return "Compute the spectral spread of the signal for each frame. The unit of the values returned is Hz. If the frame is completely silent a value of 0 is returned.";
}

string
SpectralSpread::getMaker() const
{
    // Your name here
    return "MIR.EDU by Justin Salamon";
}

int
SpectralSpread::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
SpectralSpread::getCopyright() const
{
    // This function is not ideally named.  It does not necessarily
    // need to say who made the plugin -- getMaker does that -- but it
    // should indicate the terms under which it is distributed.  For
    // example, "Copyright (year). All Rights Reserved", or "GPL"
    return "GPL";
}

SpectralSpread::InputDomain
SpectralSpread::getInputDomain() const
{
    return FrequencyDomain;
}

size_t
SpectralSpread::getPreferredBlockSize() const
{
    return 0; // 0 means "I can handle any block size"
}

size_t 
SpectralSpread::getPreferredStepSize() const
{
    return 0; // 0 means "anything sensible"; in practice this
              // means the same as the block size for TimeDomain
              // plugins, or half of it for FrequencyDomain plugins
}

size_t
SpectralSpread::getMinChannelCount() const
{
    return 1;
}

size_t
SpectralSpread::getMaxChannelCount() const
{
    return 1;
}

SpectralSpread::ParameterList
SpectralSpread::getParameterDescriptors() const
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
SpectralSpread::getParameter(string identifier) const
{
   // if (identifier == "parameter") {
   //     return 5; // return the ACTUAL current value of your parameter here!
   // }
    return 0;
}

void
SpectralSpread::setParameter(string identifier, float value) 
{
    //if (identifier == "parameter") {
    //    // set the actual value of your parameter
    //}
}

SpectralSpread::ProgramList
SpectralSpread::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
SpectralSpread::getCurrentProgram() const
{
    return ""; // no programs
}

void
SpectralSpread::selectProgram(string name)
{
}

SpectralSpread::OutputList
SpectralSpread::getOutputDescriptors() const
{
    OutputList list;

    // See OutputDescriptor documentation for the possibilities here.
    // Every plugin must have at least one output.

    OutputDescriptor d;
    d.identifier = "spectralspread";
    d.name = "Spectral Spread";
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
SpectralSpread::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
    channels > getMaxChannelCount()) return false;

    // Real initialisation work goes here!
    m_blockSize = blockSize;
    m_stepSize = stepSize;

    return true;
}

void
SpectralSpread::reset()
{
    // Clear buffers, reset stored values, etc
}

SpectralSpread::FeatureSet
SpectralSpread::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    // Step 1: compute the mean (centroid)
    float magnitude_sum = 0.0f;
    float weighted_frequency_sum = 0.0f;
    vector<float> mags(m_blockSize/2);
    vector<float> frqs(m_blockSize/2);

    for (size_t i=0; i<m_blockSize; i+=2)
    {
        float breal = inputBuffers[0][i];
        float bimag = inputBuffers[0][i+1];
        mags[i/2] = sqrt(breal*breal + bimag*bimag) / (m_blockSize/4);
        frqs[i/2] = (i/2) * (float)m_inputSampleRate / (float)m_blockSize;
        magnitude_sum += mags[i/2];
        weighted_frequency_sum += frqs[i/2] * mags[i/2];
    }

    float spectralspread;

    // If signal completely silent, return 0
    if (magnitude_sum == 0)
        spectralspread = 0;
    else
    {
        float centroid = weighted_frequency_sum / magnitude_sum;
        float spread_sum = 0.0f;

        for (size_t i=0; i<m_blockSize/2; i++)
            spread_sum += pow((frqs[i]- centroid),2) * mags[i];
        
        
        spectralspread = sqrt(spread_sum / magnitude_sum); // = weighted std. deviation
    }

    Feature f;
    f.hasTimestamp = false;
    f.values.push_back(spectralspread);

    FeatureSet fs;
    fs[0].push_back(f);

    return fs;
}

SpectralSpread::FeatureSet
SpectralSpread::getRemainingFeatures()
{
    return FeatureSet();
}

