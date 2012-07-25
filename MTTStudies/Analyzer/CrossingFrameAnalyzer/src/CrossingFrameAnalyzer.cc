// -*- C++ -*-
//
// Package:    CrossingFrameAnalyzer
// Class:      CrossingFrameAnalyzer
// 
/**\class CrossingFrameAnalyzer CrossingFrameAnalyzer.cc Analyzer/CrossingFrameAnalyzer/src/CrossingFrameAnalyzer.cc

 Description: Analyze the MttSimHit Information in CrossingFrames

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Paul Maanen
//         Created:  Mon Jun 18 12:01:12 CEST 2012
// $Id: CrossingFrameAnalyzer.cc,v 1.1 2012/07/20 11:57:20 pmaanen Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


#include "FWCore/ParameterSet/interface/ParameterSet.h"

 #include "SimDataFormats/CrossingFrame/interface/MixCollection.h"

#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "TH1F.h"
#include "TH2F.h"
//
// class declaration
//

class CrossingFrameAnalyzer : public edm::EDAnalyzer {
   public:
      explicit CrossingFrameAnalyzer(const edm::ParameterSet&);
      ~CrossingFrameAnalyzer();

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
      std::map<std::string,TH1*> theHistoMap;
};

//
// constants, enums and typedefs
//
typedef std::map<uint32_t,std::vector<PSimHit> > HitMap;
//
// static data member definitions
//

//
// constructors and destructor
//
CrossingFrameAnalyzer::CrossingFrameAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
	  edm::Service<TFileService> fs;
	  	 theHistoMap["particleType"]=fs->make<TH1F>("pdgId","particleType",31,-15.5,15.5);
	  	 theHistoMap["nHitsVsNEvts"]=fs->make<TH2F>("nHitsVsnEvts","nHitsVsnEvts",1001,-0.5,1000.5,301,-0.5,300.5);
	  	 theHistoMap["eloss"]=fs->make<TH1F>("eloss","Energy loss of other particles in GeV",100,0,0.005);
	  	 theHistoMap["elossn"]=fs->make<TH1F>("eloss","Energy loss of other particles in GeV",100,0,0.005);
	  	 theHistoMap["elossmu"]=fs->make<TH1F>("eloss","Energy loss of Muons in GeV",100,0,0.005);
}


CrossingFrameAnalyzer::~CrossingFrameAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
CrossingFrameAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;


   const std::string subdet("g4SimHitsMTTHits");
   edm::Handle<CrossingFrame<PSimHit> > cf_simhit;
   iEvent.getByLabel("mix",subdet,cf_simhit);
   std::auto_ptr<MixCollection<PSimHit> > col(new MixCollection<PSimHit>(cf_simhit.product()));
   MixCollection<PSimHit>::iterator cfi;
   std::map<uint32_t,std::vector<PSimHit> > HitMap;
   std::map<uint32_t,double> EnergyMap;
int count=0;
   for (cfi=col->begin(); cfi!=col->end();cfi++) {
     //  std::cout<<" Hit "<<count<<" has tof "<<cfi->timeOfFlight()<<" trackid "<<cfi->trackId() <<" bunchcr "<<cfi.bunch()<<" trigger "<<cfi.getTrigger()<<", from EncodedEventId: "<<cfi->eventId().bunchCrossing()<<" " <<cfi->eventId().event() <<" bcr from MixCol "<<cfi.bunch()<<std::endl;
       //      std::cout<<" Hit: "<<(*cfi)<<std::endl;
	   theHistoMap["particleType"]->Fill(cfi->particleType());
	   HitMap[cfi->detUnitId()].push_back(*cfi);
//	   	   if(abs(cfi->particleType()==13))
//		   theHistoMap["elossmu"]->Fill(cfi->energyLoss());
//	   else if(abs(cfi->particleType())==2112)
//		   theHistoMap["elossn"]->Fill(cfi->energyLoss());
//	   else
//	   theHistoMap["eloss"]->Fill(cfi->energyLoss());


       count++;
     }
   std::map<uint32_t,bool> MuonMap;
   for(std::map<uint32_t,std::vector<PSimHit> >::iterator iTile=HitMap.begin();iTile!=HitMap.end();++iTile) {
       MuonMap[iTile->first]=false;
	   float totalEnergyLoss=0;
	   for(std::vector<PSimHit>::iterator iHit=iTile->second.begin();iHit!=iTile->second.end();++iHit) {
		   totalEnergyLoss+=iHit->energyLoss();
	   if(iHit->particleType()==13)
		   MuonMap[iTile->first]=true;
	   }
	EnergyMap[iTile->first]=totalEnergyLoss;
   }
   for(std::map<uint32_t,std::vector<PSimHit> >::iterator iTile=HitMap.begin();iTile!=HitMap.end();++iTile) {
	   if(MuonMap[iTile->first])
		   theHistoMap["elossmu"]->Fill(EnergyMap[iTile->first]);
	   else
		   theHistoMap["eloss"]->Fill(EnergyMap[iTile->first]);
   }

   theHistoMap["nHitsVsNEvts"]->Fill(count,col->size());

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
CrossingFrameAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CrossingFrameAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
CrossingFrameAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
CrossingFrameAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
CrossingFrameAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
CrossingFrameAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CrossingFrameAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CrossingFrameAnalyzer);
