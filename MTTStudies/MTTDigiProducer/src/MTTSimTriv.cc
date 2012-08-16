#include "MTTStudies/Geometry/interface/MTTTile.h"
#include "MTTStudies/MTTDigiProducer/interface/MTTSimTriv.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"
#include <cmath>
#include <utility>
#include <map>

//#include "CLHEP/config/CLHEP.h"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandPoissonQ.h"

MTTSimTriv::MTTSimTriv(const edm::ParameterSet& config) :
	MTTSim(config) {

}

void MTTSimTriv::setRandomEngine(CLHEP::HepRandomEngine& eng) {

}

MTTSimTriv::~MTTSimTriv() {

}

void MTTSimTriv::simulate(const MTTTile* tile, const edm::PSimHitContainer& mttHits) {


	theMttDigiSimLinks.clear();
	theDetectorHitMap.clear();
	theMttDigiSimLinks = MTTDigiSimLinks(tile->id().rawId());

	for (edm::PSimHitContainer::const_iterator _hit = mttHits.begin(); _hit != mttHits.end(); ++_hit) {

		int digi = tile->id().rawId();

		theDetectorHitMap.insert(DetectorHitMap::value_type(digi, &(*_hit)));
		tiles.insert(digi);
	}
}
