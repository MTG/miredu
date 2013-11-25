
/* MIR.EDU vamp library
 *
 * Feature name: Spectral Roll-off
 * Description: Compute the spectral roll-off of each frame of the signal,
 * defined as the frequency f_r below which 95% percent of the
 * signal energy is contrained. Note, we define this threshold as
 * a parameter that the user can change. The default value is 95%.
 * If a frame is completely silent, a value of 0 is returned.
 * Unit: Hz
 * Formula: f_r, where sum(a_i^2) from f_0..f_r = 0.95 * sum(a_i^2) from f_1 to f_N
 * where i=1..N
 * Reference: http://asadl.org/jasa/resource/1/jasman/v130/i5/p2902_s1/cart.do
 * PDF: http://mt.music.mcgill.ca/mpcl/publications/peeters-giordano-susini-misdariis-mcadams-2011
 */


// Remember to use a different guard symbol in each header!
#ifndef _SPECTRALROLLOFF_H_
#define _SPECTRALROLLOFF_H_

#include <vector>
#include <cmath>
#include <ostream>
#include <vamp-sdk/Plugin.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;

class SpectralRolloff : public Vamp::Plugin
{
public:
    SpectralRolloff(float inputSampleRate);
    virtual ~SpectralRolloff();

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
	float m_rolloffthreshold;
};


#endif // _SPECTRALROLLOFF_H_
