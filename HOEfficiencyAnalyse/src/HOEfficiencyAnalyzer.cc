// -*- C++ -*-
//
// Package:    HOEfficiencyAnalyzer
// Class:      HOEfficiencyAnalyzer
// 
/**\class HOEfficiencyAnalyzer HOEfficiencyAnalyzer.cc ho_efficiency/HOEfficiencyAnalyzer/src/HOEfficiencyAnalyzer.cc

 Description: [one line class summary]

 Implementation:
 [Notes on implementation]
 */
//
// Original Author:  Yusuf Erdogan
//         Created:  Mon Oct 15 17:56:02 CEST 2012
// $Id$
//
//


// system include files
#include <memory>

// user include files

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <DataFormats/PatCandidates/interface/Muon.h>
#include <DataFormats/Common/interface/RefVectorIterator.h>
#include <DataFormats/GeometryVector/interface/Basic3DVector.h>
#include <DataFormats/HcalDetId/interface/HcalDetId.h>
#include <DataFormats/MuonReco/interface/MuonTrackLinks.h>
#include <DataFormats/GeometryVector/interface/Point3DBase.h>
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputer.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputerRcd.h"

#include "RecoMuon/MuonIdentification/interface/MuonHOAcceptance.h"

#include <TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h>
#include <TrackingTools/TrajectoryState/interface/TrajectoryStateClosestToPoint.h>
#include <TrackingTools/TransientTrack/interface/TransientTrack.h>
#include <TrackingTools/TransientTrack/interface/TransientTrackBuilder.h>
#include <TrackingTools/Records/interface/TransientTrackRecord.h>
#include <TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h>
#include <TrackingTools/TrackAssociator/interface/TrackDetMatchInfo.h>

#include "TH2F.h"
#include "TH2I.h"
#include "TGraph.h"

//
// class declaration
//

class HOEfficiencyAnalyzer: public edm::EDAnalyzer {
public:
	explicit HOEfficiencyAnalyzer(const edm::ParameterSet&);
	~HOEfficiencyAnalyzer();

	static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
	virtual void beginJob();
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob();

	virtual void beginRun(edm::Run const&, edm::EventSetup const&);
	virtual void endRun(edm::Run const&, edm::EventSetup const&);
	virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
	virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

	// ----------member data ---------------------------

	std::map<std::string, TH1*> histos_;
	edm::Service<TFileService> theFileService;
	TFile* theOutputFile;

	const CaloGeometry *theGeometry;
	const CaloCellGeometry *theCellGeometry;
	const CaloSubdetectorGeometry *theSubDetGeometry;
	TrajectoryStateOnSurface thestateOnSurface1;
	TrajectoryStateOnSurface thestateOnSurface2;
	TrajectoryStateOnSurface thestateOnSurface3;

	TrajectoryStateClosestToPoint theStateClosestToPoint1;
	TrajectoryStateClosestToPoint theStateClosestToPoint2;
	TrajectoryStateClosestToPoint theStateClosestToPoint3;

	StringCutObjectSelector<reco::Muon> selector_;

	TrackDetectorAssociator theTrackDetectorAssociator;
	TrackAssociatorParameters trackAssociatorParameters;

	MuonHOAcceptance theMuonHOAcceptance;
};

//
// constants, enums and typedefs
//

typedef std::map<uint32_t, edm::SortedCollection<HORecHit, edm::StrictWeakOrdering<HORecHit>>>HitMap;
//typedef edm::SortedCollection<HORecHit> HOHitCollection;
typedef edm::SortedCollection<HORecHit, edm::StrictWeakOrdering<HORecHit>> HOHitCollection;

typedef math::XYZTLorentzVector LorentzVector;
typedef math::XYZVector Vector;

edm::InputTag beamspotLabel_;
edm::InputTag primvertexLabel_;

HcalDetId hitDetId;
HcalDetId hpdHitDetId;
HcalDetId sipmHitDetId;

//
// static data member definitions
//

//
// constructors and destructor
//
HOEfficiencyAnalyzer::HOEfficiencyAnalyzer(const edm::ParameterSet& iConfig) :
	selector_(iConfig.getParameter<std::string> ("selection"))

{

	//now do what ever initialization is needed

	beamspotLabel_ = iConfig.getParameter<edm::InputTag> ("beamSpot");
	primvertexLabel_ = iConfig.getParameter<edm::InputTag> ("primaryVertex");

	//edm::ParameterSet parameters = iConfig.getParameter<edm::ParameterSet> ("TrackAssociatorParameters");
	//trackAssociatorParameters.loadParameters(parameters);
	//theTrackDetectorAssociator.useDefaultPropagator();

	//	histos_["inner_match"] = theFileService->make<TH1F> ("inner_match", "inner_match", 2, -0.5, 1.5);
	//	histos_["outer_match"] = theFileService->make<TH1F> ("outer_match", "outer_match", 2, -0.5, 1.5);
	//	histos_["global_match"] = theFileService->make<TH1F> ("global_match", "global_match", 2, -0.5, 1.5);

	histos_["eta_phi_match"] = theFileService->make<TH1F> ("eta_phi_matchedhits", "eta_phi_matchedhits", 2, -0.5, 1.5);
	histos_["eta_phi_multimatch"] = theFileService->make<TH1F> ("eta_phi_multimatchedhits", "eta_phi_multimatchedhits",
			2, -0.5, 1.5);

	histos_["muons_freq"] = theFileService->make<TH1F> ("muons_freq", "muons_freq", 30, -0.5, 29.5);
	histos_["hit_freq"] = theFileService->make<TH1F> ("hit_freq", "hit_freq", 300, 0, 300);

	histos_["muons_pt"] = theFileService->make<TH1F> ("muons_pt", "muons_pt", 100, 0, 200.);
	histos_["muons_phi"] = theFileService->make<TH1F> ("muons_phi", "muons_phi", 80, -4., 4.);
	histos_["muons_eta"] = theFileService->make<TH1F> ("muons_eta", "muons_eta", 200, -2.0, 2.0);

	histos_["s1vec_phi"] = theFileService->make<TH1F> ("s1vec_phi", "s1vec_phi", 80, -4., 4.);
	histos_["s1vec_eta"] = theFileService->make<TH1F> ("s1vec_eta", "s1vec_eta", 28, -1.4, 1.4);
	//histos_["hit_energy"] = theFileService->make<TH1F> ("hpdHit_energy", "hpdHit_energy", 100, 0., 5.);


	histos_["dR"] = theFileService->make<TH1F> ("deltaR", "deltaR", 1000, 0., 10.);
	//	histos_["control1"] = theFileService->make<TH1F> ("control1", "control1", 100, 0., 100.);
	//	histos_["control2"] = theFileService->make<TH1F> ("control2", "control2", 100, 0., 100.);
	histos_["control3"] = theFileService->make<TH1F> ("control3", "control3", 100, 0., 100.);

	//	histos_["dist2"] = theFileService->make<TH1F> ("dist2", "dist2", 100, 0., 500.);
	histos_["dist3"] = theFileService->make<TH1F> ("dist3", "dist3", 100, 0., 1000.);

}

HOEfficiencyAnalyzer::~HOEfficiencyAnalyzer() {

	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)
}

// ------------ method called for each event  ------------
void HOEfficiencyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

	using namespace edm;

	Handle<HOHitCollection> hohits;
	Handle<std::vector<reco::Muon>> muons;

	iEvent.getByLabel("horeco", hohits);
	iEvent.getByLabel("MuonSelector", muons);

	ESHandle<CaloGeometry> caloGeom;
	ESHandle<TransientTrackBuilder> theB;

	iSetup.get<TransientTrackRecord> ().get("TransientTrackBuilder", theB);
	iSetup.get<CaloGeometryRecord> ().get(caloGeom);

	theGeometry = caloGeom.product();

	HitMap theHitMap;

	histos_["hit_freq"]->Fill(hohits->size());

	theMuonHOAcceptance.initIds(iSetup);

	//-------------------------------Vertex information------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	reco::Vertex::Point posVtx;
	reco::Vertex::Error errVtx;

	edm::Handle<reco::VertexCollection> recVtxs;
	iEvent.getByLabel(primvertexLabel_, recVtxs);

	unsigned int theIndexOfThePrimaryVertex = 999.;

	for (unsigned int ind = 0; ind < recVtxs->size(); ++ind) {
		if ((*recVtxs)[ind].isValid() && !((*recVtxs)[ind].isFake())) {
			theIndexOfThePrimaryVertex = ind;
			break;
		}
	}

	if (theIndexOfThePrimaryVertex < 100) {
		posVtx = ((*recVtxs)[theIndexOfThePrimaryVertex]).position();
		errVtx = ((*recVtxs)[theIndexOfThePrimaryVertex]).error();
	} else {
		LogInfo("RecoMuonValidator") << "reco::PrimaryVertex not found, use BeamSpot position instead\n";
		edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
		iEvent.getByLabel(beamspotLabel_, recoBeamSpotHandle);
		reco::BeamSpot bs = *recoBeamSpotHandle;
		posVtx = bs.position();
		errVtx(0, 0) = bs.BeamWidthX();
		errVtx(1, 1) = bs.BeamWidthY();
		errVtx(2, 2) = bs.sigmaZ();
	}

	const reco::Vertex thePrimaryVertex(posVtx, errVtx);

	const GlobalPoint vertex(thePrimaryVertex.x(), thePrimaryVertex.y(), thePrimaryVertex.z());

	//-------------------------------------------------------------------------------------------------------

	if (muons->size() > 0) {
		for (std::vector<reco::Muon>::const_iterator muon_iter = muons->begin(); muon_iter != muons->end(); ++muon_iter) {

			bool isTightMu = muon::isTightMuon(*muon_iter, thePrimaryVertex);

			if (!isTightMu) continue;

			//if(available == 1){
			//	reco::TransientTrack tr_muonglobaltrack = (*theB).build((muon_global_ref));
			//
			//	theStateClosestToPoint3 = tr_muonglobaltrack.trajectoryStateClosestToPoint(s1);
			//
			//	if(theStateClosestToPoint3.isValid()){
			//		GlobalPoint closestPoint3 = theStateClosestToPoint3.position();
			//		GlobalPoint refPoint3 = theStateClosestToPoint3.referencePoint();
			//
			//		GlobalVector closest3Vec(closestPoint3.x(), closestPoint3.y(), closestPoint3.z());
			//		GlobalVector ref3Vec(refPoint3.x(), refPoint3.y(), refPoint3.z());
			//
			//		GlobalVector controldist3 = s1vec - ref3Vec;
			//		histos_["control3"]->Fill(controldist3.mag());
			//
			//		GlobalVector dist3 = s1vec - closest3Vec;
			//		histos_["dist3"]->Fill(dist3.mag());
			//	}
			//}

			// kinematic parameters from p4
			double muon_phi = muon_iter->phi();
			double muon_eta = muon_iter->eta();
			double muon_pt = muon_iter->pt();

			//bool inGeomAcceptance = theMuonHOAcceptance.inGeomAccept(muon_eta, muon_phi, 0., 0.);
			//bool inNotDead = theMuonHOAcceptance.inNotDeadGeom(muon_eta, muon_phi, 0., 0.);

			//if(!inGeomAcceptance) continue;
			//if(!inNotDead) continue;

			bool available = 0;
			reco::TrackRef muon_global_ref;

			if (muon_iter->isGlobalMuon()) {
				muon_global_ref = muon_iter->globalTrack();
				if (muon_global_ref.isAvailable())
					available = 1;
			}

			if (available == 1) {
				const reco::Track* muon_global_track = muon_global_ref.get();
				//TrackDetMatchInfo info = theTrackDetectorAssociator.associate(iEvent, iSetup,
				//		theTrackDetectorAssociator.getFreeTrajectoryState(iSetup, *muon_global_track),
				//		trackAssociatorParameters);


			}

			histos_["muons_pt"]->Fill(muon_pt);
			histos_["muons_phi"]->Fill(muon_phi);
			histos_["muons_eta"]->Fill(muon_eta);

			int matchedhits1 = 0;
			//int matchedhits2 = 0;


			for (HOHitCollection::const_iterator iHit = hohits->begin(); iHit != hohits->end(); ++iHit) {

				hitDetId = iHit->id();

				//bool isDeadChannel = theMuonHOAcceptance.isChannelDead(hitDetId);
				//bool isSiPMChannel = theMuonHOAcceptance.isChannelSiPM(hitDetId);

				//if(isDeadChannel) continue;

				//int ieta = hitDetId.ieta();
				//int iphi = hitDetId.iphi();

				GlobalPoint s1 = theGeometry->getPosition(hitDetId);
				GlobalVector s1vec(s1.x(), s1.y(), s1.z());

				double s1vec_phi = s1vec.phi();
				double s1vec_eta = s1vec.eta();

				histos_["s1vec_phi"]->Fill(s1vec_phi);
				histos_["s1vec_eta"]->Fill(s1vec_eta);

				double deltaPhi1 = s1vec_phi - muon_phi;

				if (deltaPhi1 >= 3.1412) {
					deltaPhi1 -= 2 * 3.1412;
				} else if (deltaPhi1 < -3.1412)
					deltaPhi1 += 2 * 3.1412;

				double deltaEta1 = s1vec_eta - muon_eta;

				if (fabs(deltaEta1) <= 0.0435 && fabs(deltaPhi1) <= 0.0435) {
					matchedhits1++;
				}
			}

			bool eta_phi_match = 0;
			bool eta_phi_multimatch = 0;

			if (matchedhits1 == 1) {
				eta_phi_match = 1;
			} else if (matchedhits1 > 1) {
				eta_phi_multimatch = 1;
			}

			histos_["eta_phi_match"]->Fill(eta_phi_match);
			histos_["eta_phi_multimatch"]->Fill(eta_phi_multimatch);
		}
		histos_["muons_freq"]->Fill(muons->size());
	}

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
void HOEfficiencyAnalyzer::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void HOEfficiencyAnalyzer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void HOEfficiencyAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void HOEfficiencyAnalyzer::endRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void HOEfficiencyAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void HOEfficiencyAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void HOEfficiencyAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE( HOEfficiencyAnalyzer);
