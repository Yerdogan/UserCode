// -*- C++ -*-
//
// Package:    SimpleSimHitAnalyzer
// Class:      SimpleSimHitAnalyzer
// 
/**\class SimpleSimHitAnalyzer SimpleSimHitAnalyzer.cc Analyzer/SimpleSimHitAnalyzer/src/SimpleSimHitAnalyzer.cc

 Description: A simple analyzer to get some ideas about the SimHits in MTT

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Paul Maanen
//         Created:  Wed May  9 15:29:39 CEST 2012
// $Id: SimpleSimHitAnalyzer.cc,v 1.1 2012/05/16 15:33:00 pmaanen Exp $
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
// constants, enums and typedefs
//
typedef std::map<uint32_t,std::vector<PSimHit> > HitMap;
typedef std::vector<PSimHit> HitCollection;
//
// class declaration
//

class SimpleSimHitAnalyzer : public edm::EDAnalyzer {
   public:
      explicit SimpleSimHitAnalyzer(const edm::ParameterSet&);
      ~SimpleSimHitAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      // ----------member data ---------------------------
  	std::map<std::string, TH1*> histos_;
  	const MTTGeometry *theGeometry;
  	edm::Service<TFileService> theFileService;
  	TGraph2D* xyzPoints;
  	//HitMap* theHitMap;
  	TFile* theOutputFile;
  	int i;
  	int events;
};
//
// static data member definitions
//

//
// constructors and destructor
//
SimpleSimHitAnalyzer::SimpleSimHitAnalyzer(const edm::ParameterSet& iConfig)

{
	i=0;
	events=0;
		if (!theFileService.isAvailable())
			throw cms::Exception("BadConfig") << "TFileService unavailable: "
					<< "please add it to config file";

		histos_["mttPlane_xy"] = theFileService->make<TH2F> ("mtt-plane-x-y",
				"mtt-plane-x-y", 1320, -660, 660, 1320, -660, 660);
		histos_["mttPlane_xy"]->GetXaxis()->SetTitle("x/cm");
		histos_["mttPlane_xy"]->GetYaxis()->SetTitle("y/cm");

		histos_["mttPlane_xz"] = theFileService->make<TH2F> ("mtt-plane-x-z",
				"mtt-plane-x-z", 1320, -660, 660, 1320, -660, 660);
		histos_["mttPlane_xz"]->GetXaxis()->SetTitle("x/cm");
		histos_["mttPlane_xz"]->GetYaxis()->SetTitle("z/cm");

		histos_["mttPlane_yz"] = theFileService->make<TH2F> ("mtt-plane-y-z",
				"mtt-plane-y-z", 1320, -660, 660, 1320, -660, 660);
		histos_["mttPlane_yz"]->GetXaxis()->SetTitle("y/cm");
		histos_["mttPlane_yz"]->GetYaxis()->SetTitle("z/cm");
		histos_["nTags_emin"]=theFileService->make<TH1F>("nTagsEmin","nTagsEmin",16,-0.5,15.5);
		histos_["nTags_mu"]=theFileService->make<TH1F>("nTagsMu","nTagsMu",16,-0.5,15.5);
		histos_["nTags"]=theFileService->make<TH1F>("nTags","nTags",16,-0.5,15.5);
		xyzPoints=new TGraph2D();
		theOutputFile=new TFile("myFile.root","RECREATE");
		theOutputFile->Append(xyzPoints);
}


SimpleSimHitAnalyzer::~SimpleSimHitAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void SimpleSimHitAnalyzer::analyze(const edm::Event& iEvent,
		const edm::EventSetup& iSetup) {
	using namespace edm;
++events;
	edm::ESHandle<MTTGeometry> mttGeom;
	iSetup.get<MuonGeometryRecord> ().get(mttGeom);
	if (!mttGeom.isValid()) {
		throw cms::Exception("Occupancy::analyze")
				<< "No valid MTT geometry found.";
	}
	theGeometry = mttGeom.product();

	edm::Handle<std::vector<PSimHit> > simHits;
	iEvent.getByLabel(edm::InputTag("g4SimHits", "MTTHits"), simHits);

	HitMap theHitMap;
	HitMap theEminHitMap;
	for (HitCollection::const_iterator iHit = simHits->begin(); iHit
			!= simHits->end(); ++iHit) {

		theHitMap[(uint32_t)iHit->detUnitId()].push_back(*iHit);
		if(iHit->energyLoss()>0.001)
			theEminHitMap[(uint32_t)iHit->detUnitId()].push_back(*iHit);
		DetId dId = DetId((uint32_t) iHit->detUnitId());
		MTTTileId mttId(dId);
		GlobalPoint p1 = theGeometry->idToDetUnit(dId)->toGlobal(
				iHit->entryPoint());
		++i;
		histos_["mttPlane_xy"]->Fill(p1.x(), p1.y());
		histos_["mttPlane_xz"]->Fill(p1.x(), p1.z());
		histos_["mttPlane_yz"]->Fill(p1.y(), p1.z());
		xyzPoints->SetPoint(i, p1.x(), p1.y(), p1.z());
	}
	histos_["nTags"]->Fill(theHitMap.size());
	histos_["nTags_emin"]->Fill(theEminHitMap.size());
	if (theEminHitMap.size() < 2) {
		if (theEminHitMap.size() == 0) {
			std::cout << "ALL MUONS LOST IN EVENT: "
					<< iEvent.eventAuxiliary().event() << std::endl;
		} else {
			std::cout << "MUON(S) LOST IN EVENT: "
					<< iEvent.eventAuxiliary().event() << std::endl;
		}
	}

//	delete theHitMap;

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
SimpleSimHitAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SimpleSimHitAnalyzer::endJob() 
{
	xyzPoints->Write("xyzPoints");
	theOutputFile->Write();
	theOutputFile->Close();
	std::cout<<events<<std::endl;
}

// ------------ method called when starting to processes a run  ------------
void 
SimpleSimHitAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
SimpleSimHitAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
SimpleSimHitAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
SimpleSimHitAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SimpleSimHitAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(SimpleSimHitAnalyzer);
