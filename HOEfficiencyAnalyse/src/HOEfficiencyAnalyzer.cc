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
	virtual void beginLuminosityBlock(edm::LuminosityBlock const&,
			edm::EventSetup const&);
	virtual void endLuminosityBlock(edm::LuminosityBlock const&,
			edm::EventSetup const&);

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

	TrackAssociatorParameters assocParams;
	TrackDetectorAssociator assoc;

	MuonHOAcceptance* theMuonHOAcceptance;
};

//
// constants, enums and typedefs
//

typedef std::map<uint32_t, edm::SortedCollection<HORecHit,
		edm::StrictWeakOrdering<HORecHit>>>HitMap;
//typedef edm::SortedCollection<HORecHit> HOHitCollection;
typedef edm::SortedCollection<HORecHit, edm::StrictWeakOrdering<HORecHit>>
		HOHitCollection;

typedef math::XYZTLorentzVector LorentzVector;
typedef math::XYZVector Vector;

edm::InputTag beamspotLabel_;
edm::InputTag primvertexLabel_;

HcalDetId hitDetId;
HcalDetId hpdHitDetId;
HcalDetId sipmHitDetId;

TrackAssociatorParameters assocParams;
TrackDetectorAssociator assoc;

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

	assoc.useDefaultPropagator();
	
	beamspotLabel_ = iConfig.getParameter<edm::InputTag> ("beamSpot");
	primvertexLabel_ = iConfig.getParameter<edm::InputTag> ("primaryVertex");
	assocParams = iConfig.getParameter<edm::ParameterSet>("TrackAssociatorParameters");

	histos_["crossedIdMatch_hpd"] = theFileService->make<TH1F> ("crossedIdMatch_hpd", "crossedIdMatch_hpd", 2, -0.5,
			1.5);
	histos_["crossedIdMatch_sipm"] = theFileService->make<TH1F> ("crossedIdMatch_sipm", "crossedIdMatch_sipm", 2, -0.5,
			1.5);
	        
	histos_["countsipmmuons"] = theFileService->make<TH1F> ("countsipmmuons",
			"countsipmmuons", 30, -0.5, 29.5);
	histos_["counthpdmuons"] = theFileService->make<TH1F> ("counthpdmuons",
			"counthpdmuons", 30, -0.5, 29.5);
	
	histos_["maxDepoMatch_sipm"] = theFileService->make<TH1F> ("maxDepoMatch_sipm", "maxDepoMatch_sipm", 2, -0.5,
			1.5);
	histos_["maxDepoMatch_hpd"] = theFileService->make<TH1F> ("maxDepoMatch_hpd", "maxDepoMatch_hpd", 2, -0.5,
			1.5);

	histos_["muons_freq"] = theFileService->make<TH1F> ("muons_freq",
			"muons_freq", 5, -0.5, 5.5);
	histos_["hit_freq"] = theFileService->make<TH1F> ("hit_freq", "hit_freq",
			300, 0, 300);
	histos_["mu_crossedRecHitSize"] = theFileService->make<TH1F> ("mu_crossedRecHitSize",
			"mu_crossedRecHitSize", 5, -0.5, 4.5);
	
	histos_["crossedHOIdSize"] = theFileService->make<TH1F> ("crossedHOIdSize",
			"crossedHOIdSize", 5, -0.5, 4.5);

	histos_["muons_pt"] = theFileService->make<TH1F> ("muons_pt", "muons_pt",
			100, 0, 200.);
	histos_["muons_phi"] = theFileService->make<TH1F> ("muons_phi",
			"muons_phi", 80, -4., 4.);
	histos_["muons_eta"] = theFileService->make<TH1F> ("muons_eta",
			"muons_eta", 200, -2.0, 2.0);
	
	histos_["muon_eta_2crossed"] = theFileService->make<TH1F> ("muon_eta_2crossed", "muon_eta_2crossed", 200, -2.0, 2.0);
	histos_["muon_eta_1crossed"] = theFileService->make<TH1F> ("muon_eta_1crossed", "muon_eta_1crossed", 200, -2.0, 2.0);
	histos_["muon_eta_0crossed"] = theFileService->make<TH1F> ("muon_eta_0crossed", "muon_eta_0crossed", 200, -2.0, 2.0);

	histos_["s1vec_phi"] = theFileService->make<TH1F> ("s1vec_phi",
			"s1vec_phi", 80, -4., 4.);
	histos_["s1vec_eta"] = theFileService->make<TH1F> ("s1vec_eta",
			"s1vec_eta", 28, -1.4, 1.4);
	//histos_["hit_energy"] = theFileService->make<TH1F> ("hpdHit_energy", "hpdHit_energy", 100, 0., 5.);

	histos_["muonHOCrossedEnergy"] = theFileService->make<TH1F> ("muonHOCrossedEnergy", "muonHOCrossedEnergy", 100, 0., 10.);
	histos_["muonHOnXnEnergy"] = theFileService->make<TH1F> ("muonHOnXnEnergy", "muonHOnXnEnergy", 100, 0., 10.);
}

HOEfficiencyAnalyzer::~HOEfficiencyAnalyzer() {

	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)
}

// ------------ method called for each event  ------------
void HOEfficiencyAnalyzer::analyze(const edm::Event& iEvent,
		const edm::EventSetup& iSetup) {

	using namespace edm;

	Handle<HOHitCollection> hohits;
	Handle < std::vector < reco::Muon >> muons;

	iEvent.getByLabel("horeco", hohits);
	iEvent.getByLabel("MuonSelector", muons);

	if (!hohits.isValid()) {
		return;
	}

	ESHandle < CaloGeometry > caloGeom;
	iSetup.get<CaloGeometryRecord> ().get(caloGeom);

	theGeometry = caloGeom.product();

	HitMap theHitMap;

	if (!MuonHOAcceptance::Inited())
		MuonHOAcceptance::initIds(iSetup);

	//-------------------------------Vertex information------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	reco::Vertex::Point posVtx;
	reco::Vertex::Error errVtx;

	edm::Handle < reco::VertexCollection > recVtxs;
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
		LogInfo("RecoMuonValidator")
				<< "reco::PrimaryVertex not found, use BeamSpot position instead\n";
		edm::Handle < reco::BeamSpot > recoBeamSpotHandle;
		iEvent.getByLabel(beamspotLabel_, recoBeamSpotHandle);
		reco::BeamSpot bs = *recoBeamSpotHandle;
		posVtx = bs.position();
		errVtx(0, 0) = bs.BeamWidthX();
		errVtx(1, 1) = bs.BeamWidthY();
		errVtx(2, 2) = bs.sigmaZ();
	}

	const reco::Vertex thePrimaryVertex(posVtx, errVtx);

	const GlobalPoint vertex(thePrimaryVertex.x(), thePrimaryVertex.y(),
			thePrimaryVertex.z());

	//-------------------------------------------------------------------------------------------------------

	TrackDetMatchInfo * muMatch = 0;
	
	if (muons->size() > 0) { // go on if at least one muon there
		
		histos_["hit_freq"]->Fill(hohits->size());
		
		for (std::vector<reco::Muon>::const_iterator muon_iter = muons->begin(); muon_iter
				!= muons->end(); ++muon_iter) {

			bool isTightMu = muon::isTightMuon(*muon_iter, thePrimaryVertex);

			if (!isTightMu) //analyze only tight muons
				continue;
			
			if (!muon_iter->isTrackerMuon()) continue;
			
			if (! muon_iter->track().isNull() ) {
				
				reco::Track const * track = muon_iter->track().get();
				muMatch = new TrackDetMatchInfo(assoc.associate(iEvent,iSetup,*track,assocParams));
				
				double hophi = muMatch->trkGlobPosAtHO.Phi();
				double hoeta = muMatch->trkGlobPosAtHO.Eta();
				
				
				
				// analyze only muons in geom.accept of HO
				if (!theMuonHOAcceptance->inGeomAccept(hophi, hoeta, 0., 0.)) continue;
				// analyze only muons in non dead cells of HO
				if (!theMuonHOAcceptance->inNotDeadGeom(hophi, hoeta, 0., 0.)) continue;
				
				// energy informations of muons in HO system
				double muonHOE = muMatch->crossedEnergy(TrackDetMatchInfo::HORecHits); //same as muMatch->hoCrossedEnergy()
				double muonHOES9 = muMatch->nXnEnergy(TrackDetMatchInfo::HORecHits,1);
				DetId hoMaxId = muMatch->findMaxDeposition(TrackDetMatchInfo::HORecHits,1);
				int mu_crossedRecHitSize = muMatch->crossedHORecHits.size();
				int n_crossedHOIds = muMatch->crossedHOIds.size();

				histos_["mu_crossedRecHitSize"]->Fill(mu_crossedRecHitSize);
				histos_["crossedHOIdSize"]->Fill(n_crossedHOIds);
				histos_["muonHOCrossedEnergy"]->Fill(muonHOE);
				histos_["muonHOnXnEnergy"]->Fill(muonHOES9);
				
				// kinematic parameters from p4
				double muon_phi = muon_iter->phi();
				double muon_eta = muon_iter->eta();
				double muon_pt = muon_iter->pt();
	            
				histos_["muons_pt"]->Fill(muon_pt);
				histos_["muons_phi"]->Fill(muon_phi);
				histos_["muons_eta"]->Fill(muon_eta);
				
				if (n_crossedHOIds > 1){
					histos_["muon_eta_2crossed"]->Fill(muon_eta);
				} else if (n_crossedHOIds == 1) {
					histos_["muon_eta_1crossed"]->Fill(muon_eta);
				} else {
					histos_["muon_eta_0crossed"]->Fill(muon_eta);
				}
				
				int countsipmmuons = 0;
				int counthpdmuons = 0;
				
				bool inSiPMGeom = theMuonHOAcceptance->inSiPMGeom(hophi,hoeta,0.,0.);
				
				if(inSiPMGeom){
					countsipmmuons++;
				} else counthpdmuons++;
				
				histos_["countsipmmuons"]->Fill(countsipmmuons);
				histos_["counthpdmuons"]->Fill(counthpdmuons);
				
				int maxDepoMatch_sipm = 0;
				int maxDepoMatch_hpd = 0;
				int crossedIdMatch_sipm = 0;
				bool matched1 = 0;
				int crossedIdMatch_hpd = 0;
				bool matched2 = 0;
				
				if(inSiPMGeom){
					
					for(std::vector<const HORecHit*>::const_iterator muhit = muMatch->crossedHORecHits.begin(); 	 
						    muhit != muMatch->crossedHORecHits.end(); ++muhit) { 	 
						  if ((*muhit)->id() == hoMaxId) {
							  maxDepoMatch_sipm++;
							  break;
						  }
					}
					
					histos_["maxDepoMatch_sipm"]->Fill(maxDepoMatch_sipm);
					
					for (std::vector<DetId>::const_iterator aid = muMatch->crossedHOIds.begin();
							aid != muMatch->crossedHOIds.end(); ++aid) {
						
						if (matched1) break;
						
						HcalDetId mId(aid->rawId());
						
						if((!MuonHOAcceptance::isChannelDead(mId))){
							for (HOHitCollection::const_iterator iHit = hohits->begin(); iHit
							!= hohits->end(); ++iHit) {
								hitDetId = iHit->id();
								if(hitDetId == mId){
									crossedIdMatch_sipm++;
									matched1 = 1;
									break;
								}
							}
						}
					}
					histos_["crossedIdMatch_sipm"]->Fill(crossedIdMatch_sipm);
					
				} else {
					
					for(std::vector<const HORecHit*>::const_iterator muhit = muMatch->crossedHORecHits.begin(); 	 
						    muhit != muMatch->crossedHORecHits.end(); ++muhit) { 	 
						  if ((*muhit)->id() == hoMaxId) {
							  maxDepoMatch_hpd++;
							  break;
						  }
					}
					
					histos_["maxDepoMatch_hpd"]->Fill(maxDepoMatch_hpd);
					
					for (std::vector<DetId>::const_iterator aid = muMatch->crossedHOIds.begin();
						     aid != muMatch->crossedHOIds.end(); ++aid) {
						
						if (matched2) break;
						
						HcalDetId mId(aid->rawId());
						
						if((!MuonHOAcceptance::isChannelDead(mId))){
							for (HOHitCollection::const_iterator iHit = hohits->begin(); iHit
							!= hohits->end(); ++iHit) {
								hitDetId = iHit->id();
								if(hitDetId == mId){
									crossedIdMatch_hpd++;
									matched2 = 1;
									break;
								}
							}
						}
					}
					histos_["crossedIdMatch_hpd"]->Fill(crossedIdMatch_hpd);					
				}
			}
			
			delete muMatch;
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
void HOEfficiencyAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&,
		edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void HOEfficiencyAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&,
		edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void HOEfficiencyAnalyzer::fillDescriptions(
		edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE( HOEfficiencyAnalyzer);
