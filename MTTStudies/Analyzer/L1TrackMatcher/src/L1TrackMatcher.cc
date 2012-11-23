// -*- C++ -*-
//
// Package:    L1TrackMatcher
// Class:      L1TrackMatcher
// 
/**\class L1TrackMatcher L1TrackMatcher.cc MTTStudies/L1TrackMatcher/src/L1TrackMatcher.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
 */
//
// Original Author:  Paul Maanen
//         Created:  Mon Aug 27 15:55:47 CEST 2012
// $Id: L1TrackMatcher.cc,v 1.3 2012/09/04 08:49:04 pmaanen Exp $
//
//


#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
//
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
//
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

///////////////////////
// DATA FORMATS HEADERS
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/Common/interface/Ref.h"
//
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
//
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/Common/interface/DetSetVector.h"
//
#include "SimDataFormats/SLHC/interface/StackedTrackerTypes.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
//
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Vector3D.h"
//
#include "MTTStudies/MTTDigi/interface/MTTDigiCollection.h"
#include "MTTStudies/Geometry/interface/MTTGeometry.h"
#include "MTTStudies/MTTDetId/interface/MTTTileId.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
//
#include "SimDataFormats/SLHC/interface/L1CaloCluster.h"
//
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1EmParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
//
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
//
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementPoint.h"
#include "TrackingTools/GeomPropagators/interface/HelixArbitraryPlaneCrossing.h"
////////////////////////
// FAST SIMULATION STUFF
//#include "FastSimulation/Particle/interface/RawParticle.h"
//#include "FastSimulation/BaseParticlePropagator/interface/BaseParticlePropagator.h"

////////////////////////////
// DETECTOR GEOMETRY HEADERS
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelTopologyBuilder.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/RectangularPixelTopology.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
//
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometryRecord.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometry.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerDetUnit.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerDetId.h"

////////////////
// PHYSICS TOOLS
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "RecoTracker/TkSeedGenerator/interface/FastHelix.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "RecoTauTag/TauTagTools/interface/GeneratorTau.h"
//
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementVector.h"
#include "DataFormats/GeometrySurface/interface/BoundPlane.h"
#include "SLHCUpgradeSimulations/Utilities/interface/constants.h"

//////////////
// STD HEADERS
#include <memory>
#include <string>
#include <iostream>
#include <algorithm>
#include <stdexcept>
using namespace std;
using namespace edm;
using namespace reco;
using namespace cmsUpgrades;
using namespace l1slhc;
using namespace l1extra;

#include "TrackFinder.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TGraph2D.h"
#include "TGraphAsymmErrors.h"
#include "TGraph.h"
#include "TFile.h"
//
// class declaration
//
using namespace edm;
using namespace cmsUpgrades;
using namespace l1slhc;
using namespace l1extra;

class L1TrackMatcher : public edm::EDAnalyzer {
public:
	explicit L1TrackMatcher(const edm::ParameterSet&);
	~L1TrackMatcher();

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
	TrackFinder theTrackFinder;
	edm::InputTag l1TrackInputTag;
	const MTTGeometry* theGeometry;
	std::map<std::string,TH1*> the1dHistoMap;
	std::map<std::string,TH2*> the2dHistoMap;
	TGraph2D* xyzTilePoints;
	TGraph*   xyTilePoints;
	TGraph*   xzTilePoints;
	TGraphAsymmErrors  *effptp,*effetap,*effphip;
	TFile* theOutputFile;
	int i,tracksmatchable,tracksmatched,tracksmatchedinneighbour;
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
L1TrackMatcher::L1TrackMatcher(const edm::ParameterSet& iConfig):i(0),tracksmatchable(0),tracksmatched(0),tracksmatchedinneighbour(0)
{
	l1TrackInputTag=iConfig.getParameter<edm::InputTag>("l1TrackSrc");
	edm::Service<TFileService> fs;


	xyzTilePoints=new TGraph2D();
	xyTilePoints=new TGraph();
	xzTilePoints=new TGraph();


	theOutputFile=new TFile("myFile.root","RECREATE");
	theOutputFile->Append(xyTilePoints);
	theOutputFile->Append(xyzTilePoints);
	theOutputFile->Append(xzTilePoints);

	//char histoname[20],histotitle[40];
	//all tracks
	the1dHistoMap["ptall"]=fs->make<TH1F>("ptall","simulated p_{t} of all L1Trackks",100,0,100);
	the1dHistoMap["etaall"]=fs->make<TH1F>("etaall","simulated #eta of all L1Tracks",100,-1.5,1.5);
	the1dHistoMap["phiall"]=fs->make<TH1F>("phiall","simulated #phi of all L1Tracks",100,-TMath::Pi(),TMath::Pi());
	//selected tracks
	the1dHistoMap["ptsel"]=fs->make<TH1F>("ptsel","simulated p_{t} of selected L1Tracks",100,0,100);
	the1dHistoMap["etasel"]=fs->make<TH1F>("etasel","simulated #eta of selected L1Tracks",100,-1.5,1.5);
	the1dHistoMap["phisel"]=fs->make<TH1F>("phisel","simulated #phi of all L1Tracks",100,-TMath::Pi(),TMath::Pi());
	//selected tracks with DIGI match
	the1dHistoMap["ptmatched"]=fs->make<TH1F>("ptm","simulated p_{t} of matched L1Tracks",100,0,100);
	the1dHistoMap["etamatched"]=fs->make<TH1F>("etam","simulated #eta of matched L1Tracks",100,-1.5,1.5);
	the1dHistoMap["phimatched"]=fs->make<TH1F>("phi,","simulated #phi of matched L1Tracks",100,-TMath::Pi(),TMath::Pi());
	//Number of Tracks
	the1dHistoMap["nTracks"]=fs->make<TH1F>("nTracks","#L1Tracks",21,-0.5,20.5);
	the1dHistoMap["nGenTracks"]=fs->make<TH1F>("nMuons","#GenMuons",21,-0.5,20.5);
	the1dHistoMap["nTracksSelected"]=fs->make<TH1F>("nTracksSelected","Number of selected L1Tracks",21,-0.5,20.5);
	the1dHistoMap["nTracksMatched"]=fs->make<TH1F>("nTracksMatched","Number of selected L1Tracks matched with a tile",21,-0.5,20.5);
	//Matching Control Plots
	the1dHistoMap["dR"]=fs->make<TH1F>("dR","#Delta R between l1track and tile center",50,0,.5);
	the1dHistoMap["dEta"]=fs->make<TH1F>("dEta","#Delta #eta between l1track and tile center",25,-0.1,0.1);
	the1dHistoMap["dPhi"]=fs->make<TH1F>("dPhi","#Delta #phi between l1track and tile center",100,.2,.2);

	the1dHistoMap["sector"]=fs->make<TH1F>("sector","sector of crossing point",12,0.5,12.5);
	the1dHistoMap["wheel"]=fs->make<TH1F>("wheel","wheel of crossing point",5,-2.5,2.5);
	the1dHistoMap["strip"]=fs->make<TH1F>("strip","strip of crossing point",11,-0.5,10.5);
	the1dHistoMap["tile"]=fs->make<TH1F>("tile","tile of crossing point",11,-0.5,10.5);
	//efficiencies
	the1dHistoMap["effpt"]=fs->make<TH1F>("effPt","matching efficiency over pt",100,0,100);
	the1dHistoMap["effeta"]=fs->make<TH1F>("effeta","matching efficiency over #eta",100,-1.5,1.5);
	the1dHistoMap["effphi"]=fs->make<TH1F>("effphi","matching efficiency over #phi",100,-TMath::Pi(),TMath::Pi());


	//
	//	the2dHistoMap["hitPointsXY"]=fs->make<TH2F>("hitPointsXY","hitPointsXY",1201,-600.5,600.5,1201,-600.5,600.5);
	//	the2dHistoMap["hitPointsXZ"]=fs->make<TH2F>("hitPointsXZ","hitPointsXZ",1201,-600.5,600.5,1801,-900.5,900.5);
	//	the2dHistoMap["localhitPointsXY"]=fs->make<TH2F>("lhitPointsXY","lhitPointsXY",1201,-600.5,600.5,1201,-600.5,600.5);
	//	the2dHistoMap["localhitPointsXZ"]=fs->make<TH2F>("lhitPointsXZ","lhitPointsXZ",1201,-600.5,600.5,1801,-900.5,900.5);
	//	the2dHistoMap["localhitPointsYZ"]=fs->make<TH2F>("lhitPointsYZ","lhitPointsYZ",1201,-600.5,600.5,1801,-900.5,900.5);

	the1dHistoMap["ptnotmatched"]=fs->make<TH1F>("ptm","simulated p_{t} of L1Tracks not matched to any tile",100,0,100);
	the1dHistoMap["etanotmatched"]=fs->make<TH1F>("etam","simulated #eta of L1Tracks not matched to any tile",100,-1.5,1.5);
	the1dHistoMap["phinotmatched"]=fs->make<TH1F>("phi,","simulated #phi of L1Tracks not matched to any tile",100,-TMath::Pi(),TMath::Pi());


	the2dHistoMap["tilePointsXY"]=fs->make<TH2F>("tilePointsXY","tilePointsXY",1201,-600.5,600.5,1201,-600.5,600.5);

	the1dHistoMap["nDigis"]=fs->make<TH1F>("nDigis","Number of Digis",31,-0.5,30.5);
	the1dHistoMap["ptreso"]=fs->make<TH1F>("ptreso","p_{t} resolution",51,-5.5,5.5);
	the1dHistoMap["ratio"]=fs->make<TH1F>("digitotrack","Ratio of ndigi to ntrack",25,-0.05,2.45);

	the1dHistoMap["lowptmatchedtracks"]=fs->make<TH1F>("lowptmatchedtracks","p_{t} of matched tracks with low p_{t}",101,-0.5,100.5);

	the1dHistoMap["matchtotrackratio"]=fs->make<TH1F>("matchtotrackratio","Ratio of number of matched tracks to total tracks",15,-0.05,1.45);

	the2dHistoMap["difference"]=fs->make<TH2F>("nTracksvsnMatch","Number of tracks matched with a tile vs number of tracks above threshold ",21,-0.5,20.5,21,-0.5,20.5);
	the2dHistoMap["etaphi"]=fs->make<TH2F>("etaphi","eta and phi of not matched tracks",50,-TMath::Pi(),TMath::Pi(),50,-1.5,1.5);
}


L1TrackMatcher::~L1TrackMatcher()
{

	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
L1TrackMatcher::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	edm::Handle<std::vector<PSimHit> > simHits;
	iEvent.getByLabel(edm::InputTag("g4SimHits", "MTTHits"), simHits);

	edm::Handle<std::vector<SimTrack> > simTracks;
	iEvent.getByLabel(edm::InputTag("g4SimHits", ""), simTracks);

	edm::Handle<MTTDigiCollection> digis;
	iEvent.getByLabel("simMuonMTTDigis",digis);
	MTTDigiCollection::DigiRangeIterator itr;
	std::vector<L1TkTrack_PixelDigi_> trackswomatch;
	edm::Handle<L1TkTrack_PixelDigi_Collection> l1trackHandlePD;

	iEvent.getByLabel(l1TrackInputTag, l1trackHandlePD);

	std::vector<MTTTile*> mttTiles = theGeometry->tiles();

	std::vector<MTTTile*> TilesWithDigi;

	//map SimTrack position in vector wrt SimTrackId
	//std::map<SimTrackId,i>
	std::map<unsigned int,unsigned int> SimTrackMap;
	for (std::vector<MTTTile*>::iterator r = mttTiles.begin(); r != mttTiles.end(); r++) {
		//	the2dHistoMap["tilePointsXY"]->Fill((*r)->surface().position().x(),(*r)->surface().position().y());
		MTTTileId iTile((*r)->id());
		for(MTTDigiCollection::const_iterator itr=digis->get(iTile).first;itr!=digis->get(iTile).second;++itr){
			TilesWithDigi.push_back(*r);
		}
	}
	int nGenMuonsMatchable=0;
	edm::Handle<GenParticleCollection> genParticles;
	iEvent.getByLabel("genParticles", genParticles);

	GenParticleCollection matchableMuons;
	for (reco::GenParticleCollection::const_iterator mcIter=genParticles->begin(); mcIter != genParticles->end(); mcIter++ ) {
		if(mcIter->pt()>5&&fabs(mcIter->eta())<1.25){
			nGenMuonsMatchable++;
			matchableMuons.push_back(*mcIter);
		}
	}

	the1dHistoMap["nGenTracks"]->Fill(matchableMuons.size());
	for(unsigned int i=0;i<simTracks->size();++i){
		SimTrackMap[simTracks->at(i).trackId()]=i;
	}
	//	for (PSimHitContainer::const_iterator iHit = simHits->begin(); iHit
	//	!= simHits->end(); ++iHit) {
	//		DetId dId = DetId((uint32_t) iHit->detUnitId());
	//		MTTTileId mttId(dId);
	//		if(iHit->pabs()>1.){
	//			//std::cout<<mttId<<std::endl;
	//			LocalPoint lHit(iHit->localPosition().x(),iHit->localPosition().z(),-iHit->localPosition().y());
	//			the2dHistoMap["localhitPointsXY"]->Fill(lHit.x(),lHit.y());
	//			the2dHistoMap["localhitPointsXZ"]->Fill(lHit.x(),lHit.z());
	//			the2dHistoMap["localhitPointsYZ"]->Fill(lHit.y(),lHit.z());
	//			GlobalPoint p1 = theGeometry->idToDetUnit(dId)->toGlobal(lHit);
	//			if(!theGeometry->idToDetUnit(dId)->surface().bounds().inside(lHit,LocalError(0.,0.1,0.0))) {
	//				std::cout<<"WARNING: HIT NOT IN DET UNIT. STtempIdRANGE."<<"Local coords: "<<lHit<<std::endl;
	//				continue;
	//			}
	//			++i;
	//			//xyzPoints->SetPoint(i, p1.x(), p1.y(), p1.z());
	//			if(mttId.wheel()==1)
	//				the2dHistoMap["hitPointsXY"]->Fill(p1.x(), p1.y());
	//
	//			the2dHistoMap["hitPointsXZ"]->Fill(p1.x(), p1.z());
	//		}
	//	}

	theTrackFinder.setTiles(&TilesWithDigi);
	the1dHistoMap["nDigis"]->Fill(TilesWithDigi.size());
	/// Go on only if there are L1Tracks from Pixel Digis
	L1TkTrack_PixelDigi_Collection::const_iterator iterL1Track;
	L1TkTrack_PixelDigi_Collection selectedTracks;
	for ( iterL1Track = l1trackHandlePD->begin();  iterL1Track != l1trackHandlePD->end();  ++iterL1Track ){
		try{
			the1dHistoMap["ptall"]->Fill(simTracks->at(SimTrackMap[iterL1Track->getSimTrackId()]).momentum().Pt());
			the1dHistoMap["etaall"]->Fill(simTracks->at(SimTrackMap[iterL1Track->getSimTrackId()]).momentum().Eta());
			the1dHistoMap["phiall"]->Fill(simTracks->at(SimTrackMap[iterL1Track->getSimTrackId()]).momentum().Phi());
			if(iterL1Track->isGenuine()&&iterL1Track->getMomentum().transverse()>5.&&fabs(iterL1Track->getMomentum().eta())<1.25)
				selectedTracks.push_back(*iterL1Track);
		}
		catch(out_of_range& oor){
			std::cout<<oor.what()<<std::endl;
			std::cout<<iEvent.id()<<std::endl;
			std::cout << "L1Track: " << "genuine: "<<iterL1Track->isGenuine()<<" simTrackId: "<<iterL1Track->getSimTrackId()<<std::endl;
			std::cout <<"SimTrack vector size: "<<simTracks->size()<<std::endl;
		}
	}
	the1dHistoMap["nTracks"]->Fill(l1trackHandlePD->size());
	int nTracksSelected=selectedTracks.size();
	int nTracksMatched=0;
	if ( l1trackHandlePD->size() > 0 ) {
		/// Loop over L1Tracks
		for ( iterL1Track = selectedTracks.begin();  iterL1Track != selectedTracks.end();  ++iterL1Track ) {

			the1dHistoMap["ptreso"]->Fill(simTracks->at(SimTrackMap[iterL1Track->getSimTrackId()]).momentum().Pt()-iterL1Track->getMomentum().transverse());
			tracksmatchable++;
			//	the1dHistoMap["ptall"]->Fill(iterL1Track->getMomentum().transverse());
			if(iterL1Track->getSimTrackId()) {
				the1dHistoMap["ptsel"]->Fill(simTracks->at(SimTrackMap[iterL1Track->getSimTrackId()]).momentum().Pt());
				the1dHistoMap["etasel"]->Fill(simTracks->at(SimTrackMap[iterL1Track->getSimTrackId()]).momentum().Eta());
				the1dHistoMap["phisel"]->Fill(simTracks->at(SimTrackMap[iterL1Track->getSimTrackId()]).momentum().Phi());
			}
			std::vector<uint32_t> tileIds=theTrackFinder.findCrossing(iterL1Track->getMomentum(),iterL1Track->getVertex());
			if(tileIds.size()>0){
				tileIds=theTrackFinder.getMatchedDigis(tileIds);
				if(tileIds.size()>0){
					nTracksMatched++;
					tracksmatched++;
					float dR=999;
					float dPhi=999;
					float dEta=999;
					uint32_t bestMatch=0;
					for(std::vector<uint32_t>::const_iterator iTileId=tileIds.begin();iTileId!=tileIds.end();++iTileId){
						const MTTTile* iTile=theGeometry->tile(MTTTileId(*iTileId));
						float dPhiTemp=iterL1Track->getMomentum().phi()-iTile->surface().position().phi();
						float dEtaTemp=iterL1Track->getMomentum().eta()-iTile->surface().position().eta();
						if(TMath::Sqrt(dPhiTemp*dPhiTemp+dEtaTemp*dEtaTemp)<dR){
							dR=TMath::Sqrt(dPhi*dPhi+dEta*dEta);
							dPhi=iterL1Track->getMomentum().phi()-iTile->surface().position().phi();
							dEta=iterL1Track->getMomentum().eta()-iTile->surface().position().eta();
							bestMatch=*iTileId;
						}
					}

					the1dHistoMap["dR"]->Fill(dR);
					the1dHistoMap["dEta"]->Fill(dEta);
					the1dHistoMap["dPhi"]->Fill(dPhi);

					MTTTileId myTile(bestMatch);
					the1dHistoMap["sector"]->Fill(myTile.sector());
					the1dHistoMap["wheel"]->Fill(myTile.wheel());
					the1dHistoMap["strip"]->Fill(myTile.strip());
					the1dHistoMap["tile"]->Fill(myTile.tile());

					const MTTTile* iTile=theGeometry->tile(myTile);
					float pT=iterL1Track->getMomentum().transverse();
					LogDebug("l1trackmatcher::analyze")<<"--------DIGI-Track match in:"<<iEvent.id()<<" Best Match: " << myTile <<"-------------------"<< std::endl<<"pT of Track: "<<pT<<" GeV. dR to center of tile: "<<dR<<". dPhi= " <<dPhi<<" dEta= "<<dEta<<std::endl <<"position of Tile: eta "<<iTile->surface().position().eta()<<", phi: "<<iTile->surface().position().phi()<<std::endl<<"direction of track: eta:"<<iterL1Track->getMomentum().eta()<<", phi: "<<iterL1Track->getMomentum().phi()<<std::endl<<"---------------------------------"<<std::endl;

//					for(every gen muon bla){
//						if(dR<.2)
//					}
					if(iterL1Track->getSimTrackId()) {
						the1dHistoMap["ptmatched"]->Fill(simTracks->at(SimTrackMap[iterL1Track->getSimTrackId()]).momentum().Pt());
						the1dHistoMap["etamatched"]->Fill(simTracks->at(SimTrackMap[iterL1Track->getSimTrackId()]).momentum().Eta());
						the1dHistoMap["phimatched"]->Fill(simTracks->at(SimTrackMap[iterL1Track->getSimTrackId()]).momentum().Phi());
						if(simTracks->at(SimTrackMap[iterL1Track->getSimTrackId()]).momentum().Pt()<5)
							the1dHistoMap["lowptmatchedtracks"]->Fill(iterL1Track->getMomentum().transverse());
					}
				}
			}
			else{
				trackswomatch.push_back(*iterL1Track);
			}
		}
	}
	the1dHistoMap["nTracksMatched"]->Fill(nTracksMatched);
	the1dHistoMap["nTracksSelected"]->Fill(nTracksSelected);
	the2dHistoMap["difference"]->Fill(nTracksSelected,nTracksMatched);

	for(L1TkTrack_PixelDigi_Collection::const_iterator iTrack=trackswomatch.begin();iTrack!=trackswomatch.end();++iTrack){
		float eta,phi,pt;
		eta=iTrack->getMomentum().eta();
		phi=iTrack->getMomentum().phi();
		pt=iTrack->getMomentum().transverse();
		the1dHistoMap["etanotmatched"]->Fill(phi);
		the1dHistoMap["phinotmatched"]->Fill(eta);
		the1dHistoMap["ptnotmatched"]->Fill(pt);
		the2dHistoMap["etaphi"]->Fill(phi,eta);
	}
	the1dHistoMap["ratio"]->Fill(float(TilesWithDigi.size())/float(l1trackHandlePD->size()));
	the1dHistoMap["matchtotrackratio"]->Fill(float(nTracksMatched)/float(nTracksSelected));
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
L1TrackMatcher::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1TrackMatcher::endJob() 
{
	the2dHistoMap["difference"]->GetXaxis()->SetTitle("Number of l1tracks above threshold");
	the2dHistoMap["difference"]->GetYaxis()->SetTitle("number of matched l1 tracks above threshold");
	the1dHistoMap["dR"]->GetYaxis()->SetTitle("#");
	the1dHistoMap["dEta"]->GetYaxis()->SetTitle("#");
	the1dHistoMap["dPhi"]->GetYaxis()->SetTitle("#");

	the1dHistoMap["dR"]->GetXaxis()->SetTitle("#Delta R");
	the1dHistoMap["dEta"]->GetXaxis()->SetTitle("#Delta #eta");
	the1dHistoMap["dPhi"]->GetXaxis()->SetTitle("#Delta #phi");

	the1dHistoMap["ptreso"]->GetXaxis()->SetTitle("(p_{t} of L1Track - p_{t} of SimTrack)/GeV");

	for(int i=0;i<the1dHistoMap["effpt"]->GetNbinsX();i++){
		if(the1dHistoMap["ptsel"]->GetBinContent(i)!=0)
			the1dHistoMap["effpt"]->SetBinContent(i,float(the1dHistoMap["ptmatched"]->GetBinContent(i))/float(the1dHistoMap["ptsel"]->GetBinContent(i)));
	}

	for(int i=0;i<the1dHistoMap["effeta"]->GetNbinsX();i++){
		if(the1dHistoMap["etasel"]->GetBinContent(i)!=0)
			the1dHistoMap["effeta"]->SetBinContent(i,float(the1dHistoMap["etamatched"]->GetBinContent(i))/float(the1dHistoMap["etasel"]->GetBinContent(i)));
	}

	for(int i=0;i<the1dHistoMap["effphi"]->GetNbinsX();i++){
		if(the1dHistoMap["phisel"]->GetBinContent(i)!=0)
			the1dHistoMap["effphi"]->SetBinContent(i,float(the1dHistoMap["phimatched"]->GetBinContent(i))/float(the1dHistoMap["phisel"]->GetBinContent(i)));
	}

	effptp=new TGraphAsymmErrors(the1dHistoMap["ptmatched"],the1dHistoMap["ptsel"]);
	effetap=new TGraphAsymmErrors(the1dHistoMap["etamatched"],the1dHistoMap["etasel"]);
	effphip=new TGraphAsymmErrors(the1dHistoMap["phimatched"],the1dHistoMap["phisel"]);

	effptp->Write("effptp");
	effetap->Write("effetap");
	effphip->Write("effphip");

	xyzTilePoints->Write("xyzTilePoints");
	xyTilePoints->Write("xyTilePoints");
	xzTilePoints->Write("xzTilePoints");
	theOutputFile->Write();
	theOutputFile->Close();
	for(std::map<std::string,TH1*>::iterator histo=the1dHistoMap.begin();histo!=the1dHistoMap.end();++histo){
		histo->second->GetYaxis()->SetRangeUser(0.01,1.1*histo->second->GetMaximum());
	}
	edm::LogPrint("eff. info:")<<"number of matchable tracks "<<tracksmatchable<<std::endl;
	edm::LogPrint("eff. info:")<<"number of matched tracks "<<tracksmatched<<std::endl;
	edm::LogPrint("eff. info:")<<"number of tracks matched in neighbour "<<tracksmatchedinneighbour<<std::endl;
	edm::LogPrint("eff. info:")<<"total efficiency "<<float(tracksmatched)/float(tracksmatchable)*100<<"%"<<std::endl;
}

// ------------ method called when starting to processes a run  ------------
void 
L1TrackMatcher::beginRun(edm::Run const&, edm::EventSetup const& eventSetup)
{
	edm::ESHandle<MTTGeometry> hGeom;
	eventSetup.get<MuonGeometryRecord> ().get(hGeom);
	theGeometry=&*hGeom;
	theTrackFinder.setGeometry(&*hGeom);
	//theTrackFinder.fillMap();
	//	std::vector<MTTTile*> mttTiles = theGeometry->tiles();
	//	int nTiles=0;
	//	int iPoint=0;
	//	LocalPoint corners[8];
	//	for (std::vector<MTTTile*>::iterator r = mttTiles.begin(); r != mttTiles.end(); r++) {
	//		nTiles++;
	//		LogDebug("L1TrackMatcher::beginRun")<<(*r)->id()<<std::endl;
	//		LocalPoint localhelp(0.1,0.1,0.1);
	//		LogDebug("L1TrackMatcher::beginRun") << "position of tile "<<(*r)->surface().position()<<std::endl;
	//		LogDebug("L1TrackMatcher::beginRun") << "normal vector of tile" <<(*r)->surface().normalVector()<<std::endl;
	//		if((*r)->id().wheel()!=0){
	//			the2dHistoMap["tilePointsXY"]->Fill((*r)->surface().position().x(),(*r)->surface().position().y());
	//		//	xyTilePoints->SetPoint(nTiles++, (*r)->surface().position().x(),(*r)->surface().position().y());
	//			if((*r)->id().wheel()==1&&(*r)->id().sector()%2==0&&(*r)->id().strip()==1&&(*r)->id().tile()==1){
	//				corners[0]=LocalPoint((*r)->surface().bounds().width()/2,(*r)->surface().bounds().length()/2,(*r)->surface().bounds().thickness()/2);
	//				corners[1]=LocalPoint((*r)->surface().bounds().width()/2,-(*r)->surface().bounds().length()/2,(*r)->surface().bounds().thickness()/2);
	//				corners[2]=LocalPoint(-(*r)->surface().bounds().width()/2,(*r)->surface().bounds().length()/2,(*r)->surface().bounds().thickness()/2);
	//				corners[3]=LocalPoint(-(*r)->surface().bounds().width()/2,-(*r)->surface().bounds().length()/2,(*r)->surface().bounds().thickness()/2);
	//				corners[4]=LocalPoint((*r)->surface().bounds().width()/2,(*r)->surface().bounds().length()/2,-(*r)->surface().bounds().thickness()/2);
	//				corners[5]=LocalPoint((*r)->surface().bounds().width()/2,-(*r)->surface().bounds().length()/2,-(*r)->surface().bounds().thickness()/2);
	//				corners[6]=LocalPoint(-(*r)->surface().bounds().width()/2,(*r)->surface().bounds().length()/2,-(*r)->surface().bounds().thickness()/2);
	//				corners[7]=LocalPoint(-(*r)->surface().bounds().width()/2,-(*r)->surface().bounds().length()/2,-(*r)->surface().bounds().thickness()/2);
	//				for(int i=0;i<8;i++){
	//					xyzTilePoints->SetPoint(iPoint++,(*r)->toGlobal(corners[i]).x(),(*r)->toGlobal(corners[i]).y(),(*r)->toGlobal(corners[i]).z());
	//					xyTilePoints->SetPoint(iPoint,(*r)->toGlobal(corners[i]).x(),(*r)->toGlobal(corners[i]).y());
	//					xzTilePoints->SetPoint(iPoint,(*r)->toGlobal(corners[i]).x(),(*r)->toGlobal(corners[i]).z());
	//					if((*r)->toGlobal(corners[i]).transverse()<40){
	//						LogDebug("L1TrackMatcher::beginRun")<<"Something strange in the neighbourhood...."<<std::endl;
	//						LogDebug("L1TrackMatcher::beginRun") << (*r)->id()<<std::endl;
	//					}
	//				}
	//				LogInfo("L1TrackMatcher::beginRun") << " width="<<(*r)->surface().bounds().width() << " length=" <<(*r)->surface().bounds().length()<<" thickness= "<<(*r)->surface().bounds().thickness()<<std::endl;
	//			}
	//		}
	//	}
	//	LogDebug("L1TrackMatcher::beginRun") << "Number of Tiles:"<<nTiles<<std::endl;
	//
	//	for(int iSector=1;iSector<13;++iSector) {
	//		for(int iStrip=1;iStrip<3;++iStrip) {
	////			MTTStripId myStrip(1,1,iSector,1,iStrip);
	////			std::cout<<myStrip<<std::endl;
	////			std::cout<<theGeometry->strip(myStrip)->surface().position().phi()<<std::endl;
	//		}
	//	}
}

// ------------ method called when ending the processing of a run  ------------
void 
L1TrackMatcher::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void so, ich
L1TrackMatcher::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
L1TrackMatcher::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1TrackMatcher::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TrackMatcher);
