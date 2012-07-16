#include "SimMuon/MTTDigitizer/interface/MTTSim.h"
//#include "SimMuon/MTTDigitizer/src/MTTSimSetUp.h"
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"

MTTSim::MTTSim(const edm::ParameterSet& config)
{
}

void
MTTSim::fillDigis(uint32_t tileDetId, MTTDigiCollection& digis)
{
  //  theMTTDigiSimLinks.clear();
//  std::vector<int> vdigi;
//  vdigi.clear();

  for (std::set< int >::iterator i=tiles.begin();
       i!=tiles.end(); i++){
    if(*i != -999){
      MTTDigi mttDigi(*i);
      //NCA
      digis.insertDigi(MTTTileId(tileDetId),mttDigi);
      this->addLinks(*i);
    }
  }
  tiles.clear();
}

void MTTSim::addLinks(uint32_t tile) {

  uint32_t digi(tile);
  std::pair<DetectorHitMap::iterator, DetectorHitMap::iterator> channelHitItr 
     = theDetectorHitMap.equal_range(digi);

  for( DetectorHitMap::iterator hitItr = channelHitItr.first; 
                                hitItr != channelHitItr.second; ++hitItr){
    const PSimHit * hit = (hitItr->second);

    if(hit != 0) {
      theMttDigiSimLinks.push_back( MTTDigiSimLink(digi, hit->entryPoint(),hit->momentumAtEntry(),
						   hit->timeOfFlight(),hit->energyLoss(),hit->particleType(), 
						   hit->detUnitId(), hit->trackId(), hit->eventId(), hit->processType() ) );

    }
  }
}




