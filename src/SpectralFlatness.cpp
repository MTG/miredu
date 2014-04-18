/**
 * @file SpectralFlatness.cpp
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
 * See SpectralFlatness.h for description
*/

#include "SpectralFlatness.h"

SpectralFlatness::SpectralFlatness(float inputSampleRate) :
    Plugin(inputSampleRate),
    // Also be sure to set your plugin parameters (presumably stored
    // in member variables) to their default values here -- the host
    // will not do that for you
    m_blockSize(0),
    m_stepSize(0)
{
}

SpectralFlatness::~SpectralFlatness()
{
}

string
SpectralFlatness::getIdentifier() const
{
    return "spectralflatness";
}

string
SpectralFlatness::getName() const
{
    return "MIR.EDU: Spectral Flatness";
}

string
SpectralFlatness::getDescription() const
{
    // Return something helpful here!
    return "Compute the spectral flatness of the signal for each frame. The flatness is defined as the ratio of the "
    "geometric and arithmetical means of the spectral magnitudes. The values returned range between 0 (peaky spectrum) "
    "and 1 (flat spectrum). If the frame is completely silent a value of 1 (flat spectrum) is returned.";
}

string
SpectralFlatness::getMaker() const
{
    // Your name here
    return "MIR.EDU by Justin Salamon";
}

int
SpectralFlatness::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
SpectralFlatness::getCopyright() const
{
    // This function is not ideally named.  It does not necessarily
    // need to say who made the plugin -- getMaker does that -- but it
    // should indicate the terms under which it is distributed.  For
    // example, "Copyright (year). All Rights Reserved", or "GPL"
    return "GPL";
}

SpectralFlatness::InputDomain
SpectralFlatness::getInputDomain() const
{
    return FrequencyDomain;
}

size_t
SpectralFlatness::getPreferredBlockSize() const
{
    return 0; // 0 means "I can handle any block size"
}

size_t 
SpectralFlatness::getPreferredStepSize() const
{
    return 0; // 0 means "anything sensible"; in practice this
              // means the same as the block size for TimeDomain
              // plugins, or half of it for FrequencyDomain plugins
}

size_t
SpectralFlatness::getMinChannelCount() const
{
    return 1;
}

size_t
SpectralFlatness::getMaxChannelCount() const
{
    return 1;
}

SpectralFlatness::ParameterList
SpectralFlatness::getParameterDescriptors() const
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
SpectralFlatness::getParameter(string identifier) const
{
   // if (identifier == "parameter") {
   //     return 5; // return the ACTUAL current value of your parameter here!
   // }
    return 0;
}

void
SpectralFlatness::setParameter(string identifier, float value) 
{
    //if (identifier == "parameter") {
    //    // set the actual value of your parameter
    //}
}

SpectralFlatness::ProgramList
SpectralFlatness::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
SpectralFlatness::getCurrentProgram() const
{
    return ""; // no programs
}

void
SpectralFlatness::selectProgram(string name)
{
}

SpectralFlatness::OutputList
SpectralFlatness::getOutputDescriptors() const
{
    OutputList list;

    // See OutputDescriptor documentation for the possibilities here.
    // Every plugin must have at least one output.

    OutputDescriptor d;
    d.identifier = "spectralflatness";
    d.name = "Spectral Flatness";
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
SpectralFlatness::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
    channels > getMaxChannelCount()) return false;

    // Real initialisation work goes here!
    m_blockSize = blockSize;
    m_stepSize = stepSize;

    return true;
}

void
SpectralFlatness::reset()
{
    // Clear buffers, reset stored values, etc
}

SpectralFlatness::FeatureSet
SpectralFlatness::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    /* WITH NORMALIZATION
    // Step 1: normalise magnitudes
    vector<float> mags(m_blockSize/2);
    float magnitude_sum = 0.0f;

    for (size_t i=0; i<m_blockSize; i+=2)
    {
        float breal = inputBuffers[0][i];
        float bimag = inputBuffers[0][i+1];
        mags[i/2] = sqrt(breal*breal + bimag*bimag) / (m_blockSize/4);
        magnitude_sum += mags[i/2];
    }

    float spectralflatness;

    if (magnitude_sum == 0)
        spectralflatness = 1; // completely silent signal, return 1 (flat spectrum)
    else
    {
        // Finish normalization and compute product
        float mag_log_sum = 0.0f;
        for (size_t i=0; i<m_blockSize/2; i++)
        {
            mags[i] /= magnitude_sum; // normalize magnitude
            mag_log_sum += log(mags[i]); // compute some of logarithms
            //cout << "(" << mag_log_sum << ") "; // debugging
        }

        // Step 2: calculate geometric and arithmetical means
        float arithmetical_mean = 1.0f / float(m_blockSize/2); // we've normalized the sum to 1
        float geometric_mean = exp(mag_log_sum/float(m_blockSize/2));
        
        spectralflatness = geometric_mean / arithmetical_mean;
    }
    */

    // Without magnitude normalization
    float mag_sum = 0.0f;
    float mag_log_sum = 0.0f;

    for (size_t i=2; i<m_blockSize+2; i+=2) // Skip DC component
    {
        float breal = inputBuffers[0][i];
        float bimag = inputBuffers[0][i+1];
        float mag = sqrt(breal*breal + bimag*bimag) / (m_blockSize/4);
        mag_sum += mag;
        // instead of product compute sum of logs to avoid arithmetic underflow! (numerically safer)
        mag_log_sum += log(mag);
    }

    float spectralflatness;

    if (mag_sum == 0)
        spectralflatness = 1;
    else
    {
        float arithmetical_mean = mag_sum / float(m_blockSize/2);
        float geometric_mean = exp(mag_log_sum / float(m_blockSize/2));
        spectralflatness = geometric_mean / arithmetical_mean;
    }

    Feature f;
    f.hasTimestamp = false;
    f.values.push_back(spectralflatness);

    FeatureSet fs;
    fs[0].push_back(f);

    return fs;
}

SpectralFlatness::FeatureSet
SpectralFlatness::getRemainingFeatures()
{
    return FeatureSet();
}

