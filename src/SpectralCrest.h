
/* MIR.EDU vamp library
 *
 * Feature name: Spectral Crest
 * Description: Compute the spectral crest of each frame of the signal.
 * The crest is defined as the ratio between maximum specral magnitude
 * in the frame and the arithmetical mean of the spectral magnitudes.
 * Unit: none
 * Formula: Crest = max(m_i) / (sum(m_i)*(1/K))
 * where m_i are the spectral magnitudes.
 * Reference: http://asadl.org/jasa/resource/1/jasman/v130/i5/p2902_s1/cart.do
 * PDF: http://mt.music.mcgill.ca/mpcl/publications/peeters-giordano-susini-misdariis-mcadams-2011
 */

// Remember to use a different guard symbol in each header!
#ifndef _SPECTRALCREST_H_
#define _SPECTRALCREST_H_

#include <vector>
#include <cmath>
#include <ostream>
#include <vamp-sdk/Plugin.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;

class SpectralCrest : public Vamp::Plugin
{
public:
    SpectralCrest(float inputSampleRate);
    virtual ~SpectralCrest();

    string getIdentifier() const;
    string getName() const;
    string getDescription() const;
    string getMaker() const;
    int getPluginVersion() const;
    string getCopyright() const;

    InputDomain getInputDomain() const;
    size_t getPreferredBlockSize() const;
    size_t getPreferredStepSize() const;
    size_t getMinChannelCount() const;
    size_t getMaxChannelCount() const;

    ParameterList getParameterDescriptors() const;
    float getParameter(string identifier) const;
    void setParameter(string identifier, float value);

    ProgramList getPrograms() const;
    string getCurrentProgram() const;
    void selectProgram(string name);

    OutputList getOutputDescriptors() const;

    bool initialise(size_t channels, size_t stepSize, size_t blockSize);
    void reset();

    FeatureSet process(const float *const *inputBuffers,
                       Vamp::RealTime timestamp);

    FeatureSet getRemainingFeatures();

protected:
    // plugin-specific data and methods go here
	size_t m_blockSize;
	size_t m_stepSize;
};


#endif // _SPECTRALCREST_H_
