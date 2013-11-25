// See SpectralFlux.h for description

#include "SpectralFlux.h"

SpectralFlux::SpectralFlux(float inputSampleRate) :
    Plugin(inputSampleRate),
    // Also be sure to set your plugin parameters (presumably stored
    // in member variables) to their default values here -- the host
    // will not do that for you
	m_blockSize(0),
	m_stepSize(0),
	m_sum_prev_sqr(0)
{
}

SpectralFlux::~SpectralFlux()
{
}

string
SpectralFlux::getIdentifier() const
{
    return "spectralflux";
}

string
SpectralFlux::getName() const
{
    return "MIR.EDU: Spectral Flux";
}

string
SpectralFlux::getDescription() const
{
    // Return something helpful here!
    return "Compute the spectral flux between every two consecutive frames of the signal. The flux is defined as 1 minus the normalized correlation between successive magnitude spectra. The values returned range between 0 (no change) and 1 (maximum change). For the first frame the flux is always zero. If both frames are silent flux = 0, if only one is silent flux = 1.";
}

string
SpectralFlux::getMaker() const
{
    // Your name here
    return "MIR.EDU by Justin Salamon";
}

int
SpectralFlux::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
SpectralFlux::getCopyright() const
{
    // This function is not ideally named.  It does not necessarily
    // need to say who made the plugin -- getMaker does that -- but it
    // should indicate the teSpectralFlux under which it is distributed.  For
    // example, "Copyright (year). All Rights Reserved", or "GPL"
    return "GPL";
}

SpectralFlux::InputDomain
SpectralFlux::getInputDomain() const
{
	return FrequencyDomain;
}

size_t
SpectralFlux::getPreferredBlockSize() const
{
    return 0; // 0 means "I can handle any block size"
}

size_t 
SpectralFlux::getPreferredStepSize() const
{
    return 0; // 0 means "anything sensible"; in practice this
              // means the same as the block size for TimeDomain
              // plugins, or half of it for FrequencyDomain plugins
}

size_t
SpectralFlux::getMinChannelCount() const
{
    return 1;
}

size_t
SpectralFlux::getMaxChannelCount() const
{
    return 1;
}

SpectralFlux::ParameterList
SpectralFlux::getParameterDescriptors() const
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
SpectralFlux::getParameter(string identifier) const
{
   // if (identifier == "parameter") {
   //     return 5; // return the ACTUAL current value of your parameter here!
   // }
    return 0;
}

void
SpectralFlux::setParameter(string identifier, float value) 
{
    //if (identifier == "parameter") {
    //    // set the actual value of your parameter
    //}
}

SpectralFlux::ProgramList
SpectralFlux::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
SpectralFlux::getCurrentProgram() const
{
    return ""; // no programs
}

void
SpectralFlux::selectProgram(string name)
{
}

SpectralFlux::OutputList
SpectralFlux::getOutputDescriptors() const
{
    OutputList list;

    // See OutputDescriptor documentation for the possibilities here.
    // Every plugin must have at least one output.

    OutputDescriptor d;
    d.identifier = "spectralflux";
    d.name = "Spectral Flux";
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
SpectralFlux::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
	channels > getMaxChannelCount()) return false;

    // Real initialisation work goes here!
	m_blockSize = blockSize;
	m_stepSize = stepSize;

	m_prev_mags.clear();
	m_sum_prev_sqr = 0.0f;

    return true;
}

void
SpectralFlux::reset()
{
    // Clear buffers, reset stored values, etc
	m_prev_mags.clear();
	m_sum_prev_sqr = 0.0f;
}

SpectralFlux::FeatureSet
SpectralFlux::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
	float spectralflux;
	if (m_prev_mags.empty()) // first frame
	{
		for (size_t i=0; i<m_blockSize; i+=2)
		{
			float breal = inputBuffers[0][i];
			float bimag = inputBuffers[0][i+1];
			m_prev_mags.push_back(sqrt(breal*breal + bimag*bimag));
			m_sum_prev_sqr += m_prev_mags.back() * m_prev_mags.back();
		}
		//if (m_sum_prev_sqr==0)
		//	m_sum_prev_sqr = std::numeric_limits<float>::epsilon();
		spectralflux = 0;
	}
	else // all other frames
	{
		float sum_prod = 0.0f;
		float sum_current_sqr = 0.0f;

		for (size_t i=0; i<m_blockSize; i+=2)
		{
			float breal = inputBuffers[0][i];
			float bimag = inputBuffers[0][i+1];
			float mag = (sqrt(breal*breal + bimag*bimag)) / (m_blockSize/4);
			sum_prod += m_prev_mags[i/2] * mag;
			sum_current_sqr += mag * mag;

			m_prev_mags[i/2] = mag; // replace old mag with new for next round
		}
		
		//if (sum_current_sqr==0)
		//	sum_current_sqr = std::numeric_limits<float>::epsilon();

		if (m_sum_prev_sqr == 0 || sum_current_sqr == 0)
		{
			if (m_sum_prev_sqr == 0 && sum_current_sqr == 0)  
				spectralflux = 0; // both frames silent, flux = 0
			else
				spectralflux = 1; // one frame slilent and other not, flux = 1
		}
		else
			spectralflux = 1 - sum_prod / sqrt(m_sum_prev_sqr * sum_current_sqr); // both non-silent

		// Prepare for next round
		m_sum_prev_sqr = sum_current_sqr;
	}
    
	Feature f;
    f.hasTimestamp = false;
    f.values.push_back(spectralflux);

    FeatureSet fs;
    fs[0].push_back(f);

	return fs;
}

SpectralFlux::FeatureSet
SpectralFlux::getRemainingFeatures()
{
    return FeatureSet();
}

