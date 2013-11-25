
// This is a skeleton file for use in creating your own plugin
// libraries.  Replace MyPlugin and myPlugin throughout with the name
// of your first plugin class, and fill in the gaps as appropriate.


#include <vamp/vamp.h>
#include <vamp-sdk/PluginAdapter.h>

#include "RMS.h"
#include "ZeroCrossingRate.h"
#include "SpectralCentroid.h"
#include "SpectralSpread.h"
#include "SpectralSkewness.h"
#include "SpectralKurtosis.h"
#include "SpectralRolloff.h"
#include "SpectralFlatness.h"
#include "SpectralCrest.h"
#include "SpectralFlux.h"
#include "TemporalCentroid.h"
#include "LogAttackTime.h"
#include "AttackStartEndTimes.h"

// Declare one static adapter here for each plugin class in this library.

static Vamp::PluginAdapter<RMS> RMS_PluginAdapter;
static Vamp::PluginAdapter<ZeroCrossingRate> ZeroCrossingRate_PluginAdapter;
static Vamp::PluginAdapter<SpectralCentroid> SpectralCentroid_PluginAdapter;
static Vamp::PluginAdapter<SpectralSpread> SpectralSpread_PluginAdapter;
static Vamp::PluginAdapter<SpectralSkewness> SpectralSkewness_PluginAdapter;
static Vamp::PluginAdapter<SpectralKurtosis> SpectralKurtosis_PluginAdapter;
static Vamp::PluginAdapter<SpectralRolloff> SpectralRolloff_PluginAdapter;
static Vamp::PluginAdapter<SpectralFlatness> SpectralFlatness_PluginAdapter;
static Vamp::PluginAdapter<SpectralCrest> SpectralCrest_PluginAdapter;
static Vamp::PluginAdapter<SpectralFlux> SpectralFlux_PluginAdapter;
static Vamp::PluginAdapter<TemporalCentroid> TemporalCentroid_PluginAdapter;
static Vamp::PluginAdapter<LogAttackTime> LogAttackTime_PluginAdapter;
static Vamp::PluginAdapter<AttackStartEndTimes> AttackStartEndTimes_PluginAdapter;


// This is the entry-point for the library, and the only function that
// needs to be publicly exported.

const VampPluginDescriptor *
vampGetPluginDescriptor(unsigned int version, unsigned int index)
{
    if (version < 1) return 0;

    // Return a different plugin adaptor's descriptor for each index,
    // and return 0 for the first index after you run out of plugins.
    // (That's how the host finds out how many plugins are in this
    // library.)

    switch (index) {
    case  0: return RMS_PluginAdapter.getDescriptor();
	case  1: return ZeroCrossingRate_PluginAdapter.getDescriptor();
	case  2: return SpectralCentroid_PluginAdapter.getDescriptor();
	case  3: return SpectralSpread_PluginAdapter.getDescriptor();
	case  4: return SpectralSkewness_PluginAdapter.getDescriptor();
	case  5: return SpectralKurtosis_PluginAdapter.getDescriptor();
	case  6: return SpectralRolloff_PluginAdapter.getDescriptor();
	case  7: return SpectralFlatness_PluginAdapter.getDescriptor();
	case  8: return SpectralCrest_PluginAdapter.getDescriptor();
	case  9: return SpectralFlux_PluginAdapter.getDescriptor();
	case  10: return TemporalCentroid_PluginAdapter.getDescriptor();
	case  11: return LogAttackTime_PluginAdapter.getDescriptor();
	case  12: return AttackStartEndTimes_PluginAdapter.getDescriptor();
    default: return 0;
    }
}


