// See ZeroCrossingRate.h for description

#include "ZeroCrossingRate.h"


ZeroCrossingRate::ZeroCrossingRate(float inputSampleRate) :
    Plugin(inputSampleRate),
    // Also be sure to set your plugin parameters (presumably stored
    // in member variables) to their default values here -- the host
    // will not do that for you
	m_blockSize(0),
	m_stepSize(0)
{
}

ZeroCrossingRate::~ZeroCrossingRate()
{
}

string
ZeroCrossingRate::getIdentifier() const
{
    return "zerocrossingrate";
}

string
ZeroCrossingRate::getName() const
{
    return "MIR.EDU: Zero Crossing Rate";
}

string
ZeroCrossingRate::getDescription() const
{
    // Return something helpful here!
    return "Compute the zero crossing rate of the signal for each frame (i.e. the number of times the signal changes sign). The unit of the values returned is crossings/second.";
}

string
ZeroCrossingRate::getMaker() const
{
    // Your name here
    return "MIR.EDU by Justin Salamon";
}

int
ZeroCrossingRate::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
ZeroCrossingRate::getCopyright() const
{
    // This function is not ideally named.  It does not necessarily
    // need to say who made the plugin -- getMaker does that -- but it
    // should indicate the teZeroCrossingRate under which it is distributed.  For
    // example, "Copyright (year). All Rights Reserved", or "GPL"
    return "GPL";
}

ZeroCrossingRate::InputDomain
ZeroCrossingRate::getInputDomain() const
{
    return TimeDomain;
}

size_t
ZeroCrossingRate::getPreferredBlockSize() const
{
    return 0; // 0 means "I can handle any block size"
}

size_t 
ZeroCrossingRate::getPreferredStepSize() const
{
    return 0; // 0 means "anything sensible"; in practice this
              // means the same as the block size for TimeDomain
              // plugins, or half of it for FrequencyDomain plugins
}

size_t
ZeroCrossingRate::getMinChannelCount() const
{
    return 1;
}

size_t
ZeroCrossingRate::getMaxChannelCount() const
{
    return 1;
}

ZeroCrossingRate::ParameterList
ZeroCrossingRate::getParameterDescriptors() const
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
ZeroCrossingRate::getParameter(string identifier) const
{
   // if (identifier == "parameter") {
   //     return 5; // return the ACTUAL current value of your parameter here!
   // }
    return 0;
}

void
ZeroCrossingRate::setParameter(string identifier, float value) 
{
    //if (identifier == "parameter") {
    //    // set the actual value of your parameter
    //}
}

ZeroCrossingRate::ProgramList
ZeroCrossingRate::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
ZeroCrossingRate::getCurrentProgram() const
{
    return ""; // no programs
}

void
ZeroCrossingRate::selectProgram(string name)
{
}

ZeroCrossingRate::OutputList
ZeroCrossingRate::getOutputDescriptors() const
{
    OutputList list;

    // See OutputDescriptor documentation for the possibilities here.
    // Every plugin must have at least one output.

    OutputDescriptor d;
    d.identifier = "zerocrossingrate";
    d.name = "Zero Crossing Rate";
    d.description = "";
    d.unit = "Crossings/s";
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
ZeroCrossingRate::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
	channels > getMaxChannelCount()) return false;

    // Real initialisation work goes here!
	m_blockSize = blockSize;
	m_stepSize = stepSize;

    return true;
}

void
ZeroCrossingRate::reset()
{
    // Clear buffers, reset stored values, etc
}

ZeroCrossingRate::FeatureSet
ZeroCrossingRate::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    // Do actual work!
	int crossings = 0;

    size_t i = 1; // note: same type as m_blockSize

    while (i < m_blockSize)
	{
		if ((inputBuffers[0][i-1]>=0 && inputBuffers[0][i]< 0) ||
			(inputBuffers[0][i-1]< 0 && inputBuffers[0][i]>=0))
			crossings++;
        ++i;
    }

	float zerocrossingrate = (float)crossings / (float)m_blockSize; // unit: crossings/sample
	zerocrossingrate *= m_inputSampleRate; // unit: crossings/second

	Feature f;
    f.hasTimestamp = false;
    f.values.push_back(zerocrossingrate);

    FeatureSet fs;
    fs[0].push_back(f);

	return fs;
}

ZeroCrossingRate::FeatureSet
ZeroCrossingRate::getRemainingFeatures()
{
    return FeatureSet();
}

