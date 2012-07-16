#ifndef MTTDigitizer_MTTSim_h
#define MTTDigitizer_MTTSim_h

#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "DataFormats/MTTDigi/interface/MTTDigiCollection.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <FWCore/Framework/interface/EventSetup.h>

#include <map>
#include <set>

#include "DataFormats/Common/interface/DetSet.h"
#include "SimDataFormats/TrackerDigiSimLink/interface/StripDigiSimLink.h"
#include "SimDataFormats/MTTDigiSimLink/src/MTTDigiSimLink.h"
#include "CLHEP/Random/RandomEngine.h"

class MTTTile;
class MTTGeometry;
//class MTTSimSetUp;
class PSimHit;

class MTTSim
{
 public:

  typedef edm::DetSet<StripDigiSimLink> DigiSimLinks;
  typedef edm::DetSet<MTTDigiSimLink> MTTDigiSimLinks;

  virtual ~MTTSim(){};

  virtual void simulate(const MTTTile* tile,
			const edm::PSimHitContainer& mttHits)=0;

//  virtual void simulateNoise(const MTTTile* tile)=0;

  virtual void setRandomEngine(CLHEP::HepRandomEngine& eng)=0;

  virtual void fillDigis(uint32_t tileDetId, MTTDigiCollection& digis);

  //TODO simsetup required?
//  void setMTTSimSetUp(MTTSimSetUp* setup){theSimSetUp = setup;}
//  MTTSimSetUp* getMTTSimSetUp(){ return theSimSetUp; }

  const DigiSimLinks & digiSimLinks() const {return theDigiSimLinks;}
  const MTTDigiSimLinks & mttDigiSimLinks() const {return theMttDigiSimLinks;}

 protected:
  MTTSim(const edm::ParameterSet& config);
  virtual void init()=0;

 protected:
  std::set< int > tiles;

  //--------NEW---------------------

  /// creates links from Digi to SimTrack
  /// disabled for now
    virtual void addLinks(uint32_t tile);

  // keeps track of which hits contribute to which channels
//    typedef std::multimap<std::pair<unsigned int,int>,const PSimHit*,std::less<std::pair<unsigned int, int> > >  DetectorHitMap;
  typedef std::multimap<uint32_t, const PSimHit*>  DetectorHitMap;

  DetectorHitMap theDetectorHitMap;
  DigiSimLinks theDigiSimLinks;
  MTTDigiSimLinks theMttDigiSimLinks;
  //--------------------------------

 protected:
//  MTTSimSetUp* theSimSetUp;
};
#endif
