// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "MTTStudies/MTTDigi/interface/MTTDigiSimLink.h"
#include "MTTStudies/MTTDigi/interface/MTTDigiCollection.h"

#include "MTTStudies/Geometry/interface/MTTGeometry.h"
#include "MTTStudies/Geometry/interface/MTTTile.h"
#include "MTTStudies/MTTDigiProducer/interface/MTTSim.h"
#include "MTTStudies/MTTDigiProducer/interface/MTTSimTriv.h"
#include "MTTStudies/MTTDigiProducer/interface/MTTSimFactory.h"

//Random Number
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "CLHEP/Random/RandomEngine.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
//
// class declaration
//
//class MTTSimSetup;

class MTTDigitizer: public edm::EDProducer {
public:
	explicit MTTDigitizer(const edm::ParameterSet& ps);
	~MTTDigitizer();

	static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

	typedef edm::DetSetVector<MTTDigiSimLink> MTTDigiSimLinks;
	typedef MTTDigitizer::MTTDigiSimLinks MTTDigitizerSimLinks;

private:
	virtual void beginJob();
	virtual void produce(edm::Event&, const edm::EventSetup&);
	virtual void endJob();

	virtual void beginRun(edm::Run&, edm::EventSetup const&);
	virtual void endRun(edm::Run&, edm::EventSetup const&);
	virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
	virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

	void simulate(const MTTTile* tile, const edm::PSimHitContainer& mttHits);

	void doAction(MixCollection<PSimHit> & simHits, MTTDigiCollection & mttDigis, MTTDigiSimLinks & mttDigiSimLink);

	// ----------member data ---------------------------
	const MTTGeometry* theGeometry;
	MTTSim* theMTTSim;
	//     const MTTSimSetup* theSetup;

	//Name of Collection used for create the XF
	std::string collection_for_XF;

	std::string theName;
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
MTTDigitizer::MTTDigitizer(const edm::ParameterSet& ps) {
	//register your products

	produces<MTTDigiCollection> ();
	produces<MTTDigitizerSimLinks> ("MTTDigiSimLink");

	//Name of Collection used for create the XF
	collection_for_XF = ps.getParameter<std::string> ("InputCollection");

	edm::Service<edm::RandomNumberGenerator> rng;
	if (!rng.isAvailable()) {
		throw cms::Exception("Configuration") << "MTTDigitizer requires the RandomNumberGeneratorService\n"
			"which is not present in the configuration file.  You must add the service\n"
			"in the configuration file or remove the modules that require it.";
	}

	theName = ps.getParameter<std::string> ("digiModel");
	theMTTSim = MTTSimFactory::get()->create(theName, ps.getParameter<edm::ParameterSet> ("digiModelConfig"));

	CLHEP::HepRandomEngine& engine = rng->getEngine();

}

MTTDigitizer::~MTTDigitizer() {

	if (theMTTSim)
		delete theMTTSim;
	theMTTSim = 0;

}

//
// member functions
//

// ------------ method called to produce the data  ------------
void MTTDigitizer::produce(edm::Event& e, const edm::EventSetup& eventSetup) {
	edm::Handle<CrossingFrame<PSimHit> > cf;
	e.getByLabel("mix", collection_for_XF, cf);

	std::auto_ptr < MixCollection<PSimHit> > hits(new MixCollection<PSimHit> (cf.product()));

	// Create empty output
	std::auto_ptr < MTTDigiCollection > pDigis(new MTTDigiCollection());
	std::auto_ptr<MTTDigitizerSimLinks> MTTDigitSimLink(new MTTDigitizerSimLinks());

	doAction(*hits, *pDigis, *MTTDigitSimLink);

	// store them in the event
	e.put(pDigis);
	e.put(MTTDigitSimLink, "MTTDigiSimLink");
}

// ------------ method called once each job just before starting event loop  ------------
void MTTDigitizer::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void MTTDigitizer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void MTTDigitizer::beginRun(edm::Run&, const edm::EventSetup& eventSetup) {

	edm::ESHandle<MTTGeometry> hGeom;
	eventSetup.get<MuonGeometryRecord> ().get(hGeom);
	theGeometry=&*hGeom;
}

// ------------ method called when ending the processing of a run  ------------
void MTTDigitizer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void MTTDigitizer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void MTTDigitizer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {
}
void MTTDigitizer::simulate(const MTTTile* tile, const edm::PSimHitContainer& mttHits) {
	/*
	 for (edm::PSimHitContainer::const_iterator _hit = mttHits.begin();
	 _hit != mttHits.end(); ++_hit){
	 if (true){
	 int time_hit = _hit->timeOfFlight();
	 std::pair<int, int> digi(topology.channel(entr)+1,
	 time_hit);

	 theDetectorHitMap.insert(DetectorHitMap::value_type(digi,&(*_hit)));
	 strips.insert(digi);
	 }

	 }
	 */
}

void MTTDigitizer::doAction(MixCollection<PSimHit> & simHits, MTTDigiCollection & mttDigis,
		MTTDigiSimLinks & mttDigiSimLink) {

	//TODO: actually digitize the hits
	// arrange the hits by tile
	std::map<int, edm::PSimHitContainer> hitMap;
	for (MixCollection<PSimHit>::MixItr hitItr = simHits.begin(); hitItr != simHits.end(); ++hitItr) {
		hitMap[hitItr->detUnitId()].push_back(*hitItr);
	}

	if (!theGeometry) {
		throw cms::Exception("Configuration")
				<< "MTTDigitizer requires the MTTGeometry \n which is not present in the configuration file.  You must add the service\n in the configuration file or remove the modules that require it.";
	}

	std::vector<MTTTile*> mttTiles = theGeometry->tiles();
	for (std::vector<MTTTile*>::iterator r = mttTiles.begin(); r != mttTiles.end(); r++) {

		const edm::PSimHitContainer & tileSimHits = hitMap[(*r)->id()];

		//    LogDebug("RPCDigitizer") << "RPCDigitizer: found " << rollSimHits.size()
		//			     <<" hit(s) in the rpc roll";

		this->simulate(*r, tileSimHits);
		//this->simulateNoise(*r);
		theMTTSim->fillDigis((*r)->id(), mttDigis);
		mttDigiSimLink.insert(theMTTSim->mttDigiSimLinks());
	}
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MTTDigitizer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(MTTDigitizer);

DEFINE_EDM_PLUGIN(MTTSimFactory,MTTSimTriv,"MTTSimTriv");
