// -*- C++ -*-
//
// Package:    MTTDetIdAnalyzer
// Class:      MTTDetIdAnalyzer
// 
/*
 Description: [one line class summary]

 Implementation:
 [Notes on implementation]
 */
//
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/MuonDetId/interface/MTTTileId.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "TH2F.h"
#include "TGraph2D.h"
#include "Geometry/MTTGeometry/interface/MTTGeometry.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <DataFormats/GeometryVector/interface/Point3DBase.h>
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/ESHandle.h"
//
// class declaration
//

class MTTDetIdAnalyzer: public edm::EDAnalyzer {
public:
	explicit MTTDetIdAnalyzer(const edm::ParameterSet&);
	~MTTDetIdAnalyzer();

	static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:

	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob();

	virtual void beginRun(edm::Run const&, edm::EventSetup const&);
	virtual void endRun(edm::Run const&, edm::EventSetup const&);
	virtual void beginLuminosityBlock(edm::LuminosityBlock const&,
			edm::EventSetup const&);
	virtual void endLuminosityBlock(edm::LuminosityBlock const&,
			edm::EventSetup const&);

	// ----------member data ---------------------------
	std::map<std::string, TH1*> histos_;
	const MTTGeometry *theGeometry;
	edm::Service<TFileService> theFileService;
	TGraph2D* xyzPoints;
	int i;
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
MTTDetIdAnalyzer::MTTDetIdAnalyzer(const edm::ParameterSet& iConfig)

{
	//now do what ever initialization is needed
}

MTTDetIdAnalyzer::~MTTDetIdAnalyzer() {

	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ method called for each event  ------------
void MTTDetIdAnalyzer::analyze(const edm::Event& iEvent,
		const edm::EventSetup& iSetup)

{

	using namespace edm;

	edm::LogInfo("HitStudy") << "Run = " << iEvent.id().run() << " Event = "
			<< iEvent.id().event();

	edm::ESHandle<MTTGeometry> mttGeom;
	iSetup.get<MuonGeometryRecord> ().get(mttGeom);
	if (!mttGeom.isValid()) {
		throw cms::Exception("Occupancy::analyze")
				<< "No valid MTT geometry found.";
	}
	theGeometry = mttGeom.product();

	edm::Handle<std::vector<PSimHit> > simHits;
	iEvent.getByLabel(edm::InputTag("g4SimHits", "MTTHits"), simHits);
	uint32_t oldId = 0;
	if (true) {
		std::cout
				<< "------------------------------------------------------------------------"
				<< std::endl;
		std::cout << "Event=" << iEvent.id().event() << std::endl;
		std::cout << "MTTGeometry " << ": Analyzer..." << std::endl;
		std::cout << "start "
				<< "-------------------------------------------------------"
				<< std::endl;
		std::cout << " Geometry node for MTTGeom is  " << &(*theGeometry)
				<< std::endl;
		std::cout << " I have " << theGeometry->detTypes().size()
				<< " detTypes" << std::endl;
		std::cout << " I have " << theGeometry->detUnits().size()
				<< " detUnits" << std::endl;
		std::cout << " I have " << theGeometry->dets().size() << " dets"
				<< std::endl;
		std::cout << " I have " << theGeometry->tiles().size() << " tiles"
				<< std::endl;
		std::cout << " I have " << theGeometry->strips().size() << " strips"
				<< std::endl;
		std::cout << " I have " << theGeometry->layers().size() << " layers"
				<< std::endl;
		std::cout << " I have " << theGeometry->panels().size() << " panels"
				<< std::endl;

	for (std::vector<PSimHit>::const_iterator iHit = simHits->begin(); iHit
			!= simHits->end(); ++iHit) {
		DetId dId = DetId((uint32_t) iHit->detUnitId());
		MTTTileId mttId(dId);
			std::cout <<"theGeometry= "<<theGeometry<<std::endl;
			std::cout <<"theGeometry->idToDet= "<<theGeometry->idToDet(mttId)<<std::endl;
			GlobalPoint  p1 = theGeometry->idToDetUnit(dId)->toGlobal(iHit->entryPoint());
			++i;
			histos_["mttPlane_xy"]->Fill(p1.x(), p1.y());
			histos_["mttPlane_xz"]->Fill(p1.x(), p1.z());
			histos_["mttPlane_yz"]->Fill(p1.y(), p1.z());
			xyzPoints->SetPoint(i,p1.x(),p1.y(),p1.z());
			std::cout<<"Coords:"<<" "<< p1.x()<<" "<<p1.y()<<" "<<p1.z()<<std::endl;
		uint32_t rawId = iHit->detUnitId();
		if (rawId == oldId)
			continue;
		MTTTileId tid(rawId);
		std::cout << "Particle Id= " << iHit->particleType() << " wheel= "
				<< tid.wheel() << " sector= " << tid.sector() << " layer= "
				<< tid.layer() << " strip= " << tid.strip() << " tile= "
				<< tid.tile() << std::endl;
		oldId = rawId;

	}
	std::cout
			<< "------------------------------------------------------------------------"
			<< std::endl;
	}
}

// ------------ method called once each job just before starting event loop  ------------
void MTTDetIdAnalyzer::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void MTTDetIdAnalyzer::endJob() {
	xyzPoints->Print("/user/maanen/graph.root");
}

// ------------ method called when starting to processes a run  ------------
void MTTDetIdAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void MTTDetIdAnalyzer::endRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void MTTDetIdAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&,
		edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void MTTDetIdAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&,
		edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MTTDetIdAnalyzer::fillDescriptions(
		edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(MTTDetIdAnalyzer)
;
