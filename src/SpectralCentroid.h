
/* MIR.EDU vamp library
 *
 * Feature name: Spectral Centroid
 * Description: Compute the spectral centroid of each frame of the signal.
 * If a frame is completely silent, a value of 0 is returned.
 * Unit: Hz
 * Formula: 
 * Reference: http://en.wikipedia.org/wiki/Spectral_centroid
 */


// Remember to use a different guard symbol in each header!
#ifndef _SPECTRALCENTROID_H_
#define _SPECTRALCENTROID_H_

#include <cmath>
#include <ostream>
#include <vamp-sdk/Plugin.h>

using std::string;
using std::cout;
using std::endl;

class SpectralCentroid : public Vamp::Plugin
{
public:
    SpectralCentroid(float inputSampleRate);
    virtual ~SpectralCentroid();

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


#endif // _SPECTRALCENTROID_H_
