// See SpectralSlope.h for description
// NOT WORKING YET

#include "SpectralSlope.h"

SpectralSlope::SpectralSlope(float inputSampleRate) :
    Plugin(inputSampleRate),
    // Also be sure to set your plugin parameters (presumably stored
    // in member variables) to their default values here -- the host
    // will not do that for you
	m_blockSize(0),
	m_stepSize(0)
{
}

SpectralSlope::~SpectralSlope()
{
}

string
SpectralSlope::getIdentifier() const
{
    return "spectralslope";
}

string
SpectralSlope::getName() const
{
    return "MIR.EDU: Spectral Slope";
}

string
SpectralSlope::getDescription() const
{
    // Return something helpful here!
    return "Compute the spectral slope of the signal for each frame. A bright spectrum will return a value close to 0. If the frame is completely silent a value of 0 is returned.";
}

string
SpectralSlope::getMaker() const
{
    // Your name here
    return "MIR.EDU by Justin Salamon";
}

int
SpectralSlope::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
SpectralSlope::getCopyright() const
{
    // This function is not ideally named.  It does not necessarily
    // need to say who made the plugin -- getMaker does that -- but it
    // should indicate the teSpectralSlope under which it is distributed.  For
    // example, "Copyright (year). All Rights Reserved", or "GPL"
    return "GPL";
}

SpectralSlope::InputDomain
SpectralSlope::getInputDomain() const
{
	return FrequencyDomain;
}

size_t
SpectralSlope::getPreferredBlockSize() const
{
    return 0; // 0 means "I can handle any block size"
}

size_t 
SpectralSlope::getPreferredStepSize() const
{
    return 0; // 0 means "anything sensible"; in practice this
              // means the same as the block size for TimeDomain
              // plugins, or half of it for FrequencyDomain plugins
}

size_t
SpectralSlope::getMinChannelCount() const
{
    return 1;
}

size_t
SpectralSlope::getMaxChannelCount() const
{
    return 1;
}

SpectralSlope::ParameterList
SpectralSlope::getParameterDescriptors() const
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
SpectralSlope::getParameter(string identifier) const
{
   // if (identifier == "parameter") {
   //     return 5; // return the ACTUAL current value of your parameter here!
   // }
    return 0;
}

void
SpectralSlope::setParameter(string identifier, float value) 
{
    //if (identifier == "parameter") {
    //    // set the actual value of your parameter
    //}
}

SpectralSlope::ProgramList
SpectralSlope::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
SpectralSlope::getCurrentProgram() const
{
    return ""; // no programs
}

void
SpectralSlope::selectProgram(string name)
{
}

SpectralSlope::OutputList
SpectralSlope::getOutputDescriptors() const
{
    OutputList list;

    // See OutputDescriptor documentation for the possibilities here.
    // Every plugin must have at least one output.

    OutputDescriptor d;
    d.identifier = "spectralslope";
    d.name = "Spectral Slope";
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
SpectralSlope::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
	channels > getMaxChannelCount()) return false;

    // Real initialisation work goes here!
	m_blockSize = blockSize;
	m_stepSize = stepSize;

    return true;
}

void
SpectralSlope::reset()
{
    // Clear buffers, reset stored values, etc
}

SpectralSlope::FeatureSet
SpectralSlope::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    // Do actual work!
	float magnitude_sum = 0.0f;
	float weighted_frequency_sum = 0.0f;
	float frequency_sum = 0.0f;
	float frequency_sqr_sum = 0.0f;

    for (size_t i=0; i<m_blockSize; i+=2)
	{
		float breal = inputBuffers[0][i];
		float bimag = inputBuffers[0][i+1];
        float magnitude = sqrt(breal*breal + bimag*bimag) / (m_blockSize/4);
		float frequency = (i/2) * (float)m_inputSampleRate / (float)m_blockSize;
        magnitude_sum += magnitude;
		weighted_frequency_sum += frequency * magnitude;
		frequency_sum += frequency;
		frequency_sqr_sum += frequency * frequency;
    }

	float spectralslope;
	if (magnitude_sum == 0)
		spectralslope = 0;
	else
	{
		int K = m_blockSize/2;
		spectralslope = (1.0 / magnitude_sum) * (K*weighted_frequency_sum - frequency_sum*magnitude_sum) / (K*frequency_sqr_sum - pow(frequency_sum,2));
		//cout << "mag_sum: " << magnitude_sum << " w_fqr_sum: " << weighted_frequency_sum << " fqr_sum: " << frequency_sum << " fqr_sqr_sum: " << frequency_sqr_sum << endl;
	}

	Feature f;
    f.hasTimestamp = false;
    f.values.push_back(spectralslope);

    FeatureSet fs;
    fs[0].push_back(f);

	return fs;
}

SpectralSlope::FeatureSet
SpectralSlope::getRemainingFeatures()
{
    return FeatureSet();
}

