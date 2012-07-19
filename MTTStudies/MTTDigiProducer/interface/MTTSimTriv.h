#ifndef MTTDigitizer_MTTSimTriv_h
#define MTTDigitizer_MTTSimTriv_h

#include "MTTStudies/MTTDigiProducer/interface/MTTSim.h"

class MTTGeometry;

namespace CLHEP {
class HepRandomEngine;
class RandFlat;
class RandPoissonQ;
}

class MTTSimTriv: public MTTSim {
public:
	MTTSimTriv(const edm::ParameterSet& config);
	~MTTSimTriv();

	void simulate(const MTTTile* tile, const edm::PSimHitContainer& mttHits);

	void setRandomEngine(CLHEP::HepRandomEngine& eng);

private:
	void init() {
	}
	;

	//  CLHEP::HepRandomEngine* rndEngine;
	CLHEP::RandFlat* flatDistribution1;
	CLHEP::RandFlat* flatDistribution2;
	CLHEP::RandPoissonQ *poissonDistribution;
};
#endif
