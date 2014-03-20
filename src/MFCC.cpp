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

    See MFCC.h for description
*/

#include "MFCC.h"

MFCC::MFCC(float inputSampleRate) :
    Plugin(inputSampleRate),
    // Also be sure to set your plugin parameters (presumably stored
    // in member variables) to their default values here -- the host
    // will not do that for you
    m_blockSize(0),
    m_stepSize(0),
    m_minFreq(0),
    m_maxFreq(inputSampleRate / 2.0),
    m_nFilters(40),
    m_nCoeffs(20),
    m_useEnergy(true)
{
}

MFCC::~MFCC()
{}

string
MFCC::getIdentifier() const
{
    return "mfcc";
}

string
MFCC::getName() const
{
    return "MIR.EDU: MFCC";
}

string
MFCC::getDescription() const
{
    // Return something helpful here!
    return "Compute the Mel Frequency Cepstral Coefficients (MFCC) for each frame.";
}

string
MFCC::getMaker() const
{
    // Your name here
    return "MIR.EDU by Justin Salamon";
}

int
MFCC::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
MFCC::getCopyright() const
{
    // This function is not ideally named.  It does not necessarily
    // need to say who made the plugin -- getMaker does that -- but it
    // should indicate the terms under which it is distributed.  For
    // example, "Copyright (year). All Rights Reserved", or "GPL"
    return "GPL";
}

MFCC::InputDomain
MFCC::getInputDomain() const
{
    return FrequencyDomain;
}

size_t
MFCC::getPreferredBlockSize() const
{
    return 0; // 0 means "I can handle any block size"
}

size_t 
MFCC::getPreferredStepSize() const
{
    return 0; // 0 means "anything sensible"; in practice this
              // means the same as the block size for TimeDomain
              // plugins, or half of it for FrequencyDomain plugins
}

size_t
MFCC::getMinChannelCount() const
{
    return 1;
}

size_t
MFCC::getMaxChannelCount() const
{
    return 1;
}

MFCC::ParameterList
MFCC::getParameterDescriptors() const
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
MFCC::getParameter(string identifier) const
{
   // if (identifier == "parameter") {
   //     return 5; // return the ACTUAL current value of your parameter here!
   // }
    return 0;
}

void
MFCC::setParameter(string identifier, float value) 
{
    //if (identifier == "parameter") {
    //    // set the actual value of your parameter
    //}
}

MFCC::ProgramList
MFCC::getPrograms() const
{
    ProgramList list;

    // If you have no programs, return an empty list (or simply don't
    // implement this function or getCurrentProgram/selectProgram)

    return list;
}

string
MFCC::getCurrentProgram() const
{
    return ""; // no programs
}

void
MFCC::selectProgram(string name)
{
}

MFCC::OutputList
MFCC::getOutputDescriptors() const
{
    OutputList list;

    // See OutputDescriptor documentation for the possibilities here.
    // Every plugin must have at least one output.

    OutputDescriptor d;
    d.identifier = "mfcc";
    d.name = "MFCC";
    d.description = "";
    d.unit = "";
    d.hasFixedBinCount = true;
    d.binCount = 13;
    d.hasKnownExtents = false;
    d.isQuantized = false;
    d.sampleType = OutputDescriptor::OneSamplePerStep;
    d.hasDuration = false;
    list.push_back(d);

    return list;
}

bool
MFCC::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
    channels > getMaxChannelCount()) return false;

    // Real initialisation work goes here!
    m_blockSize = blockSize;
    m_stepSize = stepSize;

    // Generate mel filterbanks
    m_filterbank = get_filterbanks(m_nFilters, m_blockSize, m_inputSampleRate, m_minFreq, m_maxFreq);
    
    /* DEBUG
    cout << "[";
    for (int i=0; i<m_nFilters; i++)
    {
        cout << m_filterbank[i][0];
        for (int j=1; j<(int)m_filterbank[i].size(); j++)
            cout << "," << m_filterbank[i][j];
        cout << ";";
    }
    cout << "]";
    */

    return true;
}

void
MFCC::reset()
{}

MFCC::FeatureSet
MFCC::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    // Do actual work!
    vector<float> power_spectrum(m_blockSize/2 + 1);
    float energy = 0;

    // STEP 1: compute the periodogram estimate of the power spectrum 
    for (size_t i=0; i<m_blockSize+2; i+=2)
    {
        float breal = inputBuffers[0][i];
        float bimag = inputBuffers[0][i+1];
        power_spectrum[i/2] = (breal*breal + bimag*bimag) / m_blockSize;
        energy += power_spectrum[i/2];
    }

    // STEP 2: Apply mel filterbank (see initialize function for filterbank generation)
    // This involves computing the dot product of the spectrum with each filter
    vector<float> energy_features(m_nFilters,0);
    for (size_t i=0; i<m_nFilters; i++)
        for (size_t j=0; j<m_blockSize/2+1; j++)
            energy_features[i] += power_spectrum[j] * m_filterbank[i][j];

    // STEP 3: Take the log of the features
    for (size_t i=0; i<m_nFilters; i++) {
        // add epsilon to avoid log(0) for silent frames
        energy_features[i] = log(energy_features[i] + std::numeric_limits<double>::epsilon()); 
    }

    // STEP 4: Compute the Discrete Cosine Transform (DCT) of the log energy features
    energy_features = dct(energy_features);

    // STEP 5: optionally apply liftering

    // STEP 6: optionally replace coeff0 with log of frame energy
    if (m_useEnergy)
        energy_features[0] = log(energy + std::numeric_limits<double>::epsilon());

    // STEP 7: return the desired number of coefficients
    Feature f;
    f.hasTimestamp = false;
    for (size_t k=0; k<m_nCoeffs; k++)
        f.values.push_back(energy_features[k]);

    FeatureSet fs;
    fs[0].push_back(f);

    return fs;
}

MFCC::FeatureSet
MFCC::getRemainingFeatures()
{
    return FeatureSet();
}

/**
 * Convert a value in Hertz to Mels
 * input: a value in Hz. 
 * output: a value in Mels.
 */
float
MFCC::hz2mel(float hz)
{
    return 2595 * log10(1 + hz/700.0);
}

/**
 * Convert a value in Mels to Hertz
 * input: a value in Mels. 
 * output: a value in Hz.
 */
float
MFCC::mel2hz(float mel)
{
    return 700 * (pow(10,mel/2595.0) - 1);
}

/**
 * Compute a Mel-filterbank. Each filter is stored in a vector whose length is equal to 
 * (fft length)/2. 
 *
 * Parameters: 
 *   - nfilt: the number of filters in the filterbank, default 20.
 *   - nfft: the FFT size. Default is 512.
 *   - samplerate: the samplerate of the signal we are working with. Affects mel spacing.
 *   - lowfreq: lowest band edge of mel filters, default 0 Hz
 *   - highfreq: highest band edge of mel filters, default samplerate/2
 *   
 * Output
 *   - A vector of vectors of size (nfilt, nfft/2) containing the filterbank. Each vector holds 1 filter.
 */
vector< vector<float> >
MFCC::get_filterbanks(int nfilt, int nfft, float samplerate, float lowfreq, float highfreq)
{
    highfreq = fmax(highfreq,samplerate/2);
    
    // compute points evenly spaced in mels
    float lowmel = hz2mel(lowfreq);
    float highmel = hz2mel(highfreq);
    float melstep = (highmel - lowmel) / float(nfilt + 1);
    vector<float> melpoints(nfilt + 2);
    for (int i=0; i<(int)melpoints.size(); i++) {
        melpoints[i] = lowmel + i * melstep; 
    }
        
    // our points are in Mels, but we use fft bins, so we have to convert
    // from mel to Hz to fft bin number
    for (int i=0; i<(int)melpoints.size(); i++) {
        //melpoints[i] = round(mel2hz(melpoints[i]) * nfft / samplerate);
        melpoints[i] = floor(mel2hz(melpoints[i]) * (nfft+1) / samplerate);
    }

    vector< vector<float> > filterbank(nfilt,vector<float>(nfft/2+1));
    for (int j=0; j<nfilt; j++)
    {
        // Create first half of triangle
        for (int i=int(melpoints[j]); i<int(melpoints[j+1]); i++)
            filterbank[j][i] = (i - melpoints[j]) / (melpoints[j+1]-melpoints[j]);
        
        // Create second half of triangle
        for (int i=int(melpoints[j+1]); i<int(melpoints[j+2]); i++)
            filterbank[j][i] = (melpoints[j+2]-i) / (melpoints[j+2]-melpoints[j+1]);
    }

    return filterbank;     
}

/**
 * Very basic (and inefficient) implementation of the discrete cosine transform (type II)
 */
vector<float>
MFCC::dct(vector<float> x)
{
    const float PI_F=3.14159265358979f; // hello PI :)

    int N = (int)x.size();

    // Initialize all coefficients to 0
    vector<float> dct_coeff(N,0);

    // Compute DCT using the following formula:
    // k = 0 .. N-1 (where N = size of x)
    // y(k) = w(k) * Sum_(n=0...N-1) x(n)cos(pi(2n+1)k/(2N)) where
    // w(k) = 1/sqrt(N) if k=0, 
    // w(k) = sqrt(2/N) if 1 <= k <= N-1
    for (int k=0; k<N; k++)
        for (int n=0; n<N; n++)
            dct_coeff[k] += x[n] * cos(PI_F * (2*n+1) * k / (2*N));

    dct_coeff[0] *= 1.0 / sqrt(float(N));
    for (int k=1; k<N; k++)
        dct_coeff[k] *= sqrt(2.0/float(N));

    return dct_coeff;
}


