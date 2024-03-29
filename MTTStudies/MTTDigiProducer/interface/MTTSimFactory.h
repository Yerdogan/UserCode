#ifndef MTTDigitizer_MTTSimFactory_h
#define MTTDigitizer_MTTSimFactory_h

#include "FWCore/PluginManager/interface/PluginFactory.h"
#include "MTTStudies/MTTDigiProducer/interface/MTTSim.h"

namespace edm{
  class ParameterSet;
}

//class MTTSim;

typedef edmplugin::PluginFactory<MTTSim *(const edm::ParameterSet &)> MTTSimFactory;

#endif
