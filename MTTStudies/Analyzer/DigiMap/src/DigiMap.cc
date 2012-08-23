// -*- C++ -*-
//
// Package:    DigiMap
// Class:      DigiMap
// 
/**\class DigiMap DigiMap.cc MTTStudies/DigiMap/src/DigiMap.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Paul Maanen
//         Created:  Tue Jul 24 16:18:23 CEST 2012
// $Id: DigiMap.cc,v 1.3 2012/08/16 15:49:20 pmaanen Exp $
//
//


// system include files
#include <memory>
#include <string>
#include <vector>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "MTTStudies/MTTDigi/interface/MTTDigiCollection.h"
#include "MTTStudies/Geometry/interface/MTTTile.h"
#include "MTTStudies/Geometry/interface/MTTGeometry.h"
#include "MTTStudies/MTTDetId/interface/MTTTileId.h"
#include "TH1F.h"
#include "TH2F.h"
//
// class declaration
//

class DigiMap : public edm::EDAnalyzer {
   public:
      explicit DigiMap(const edm::ParameterSet&);
      ~DigiMap();

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
      std::map<std::string,TH2*> theHistoMap;
      std::map<std::string,TH1*> the1dHistoMap;
      uint32_t	totalNumberofEvents;
  std::map<uint32_t,uint32_t> HitMap;
 const MTTGeometry* theGeometry;
};

//
// constants, enums and typedefs
//
typedef std::map<uint32_t,uint32_t> HitMap;

//
// static data member definitions
//

//
// constructors and destructor
//
DigiMap::DigiMap(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
	  edm::Service<TFileService> fs;
	  theHistoMap["DigiRate"]=fs->make<TH2F>("digirate","Rate of DIGIs",5,-2.5,2.5,12,0.5,12.5);
	  theHistoMap["nDigiVsnPU_wm2"]=fs->make<TH2F>("nDIGIvsnPU","Number of Digis vs Number of PU Vertices",61,-0.5,60.5,301,-0.5,300.5);
	  theHistoMap["nDigiVsnPU_wm1"]=fs->make<TH2F>("nDIGIvsnPU","Number of Digis vs Number of PU Vertices",61,-0.5,60.5,301,-0.5,300.5);
	  theHistoMap["nDigiVsnPU_w0"]=fs->make<TH2F>("nDIGIvsnPU","Number of Digis vs Number of PU Vertices",61,-0.5,60.5,301,-0.5,300.5);
	  theHistoMap["nDigiVsnPU_wp1"]=fs->make<TH2F>("nDIGIvsnPU","Number of Digis vs Number of PU Vertices",61,-0.5,60.5,301,-0.5,300.5);
	  theHistoMap["nDigiVsnPU_wp2"]=fs->make<TH2F>("nDIGIvsnPU","Number of Digis vs Number of PU Vertices",61,-0.5,60.5,301,-0.5,300.5);
	  the1dHistoMap["nDigi"]=fs->make<TH1F>("nDIGI","Number of Digis",61,-0.5,60.5);
}


DigiMap::~DigiMap()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
DigiMap::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	totalNumberofEvents++;
	edm::Handle<MTTDigiCollection> digis;
	Handle<std::vector< PileupSummaryInfo > >  PuInfo;
	iEvent.getByLabel(edm::InputTag("addPileupInfo"), PuInfo);
	std::vector<PileupSummaryInfo>::const_iterator PVI;

	for(PVI = PuInfo->begin(); PVI != PuInfo->end(); ++PVI) {
		if(PVI->getBunchCrossing() == 0)
		break;
		}


	//TODO: find out name for digis
	iEvent.getByLabel("simMuonMTTDigis",digis);
	MTTDigiCollection::DigiRangeIterator itr;
	//	std::cout<<"got Digis"<<std::endl;
	int numDigis[5]={0,0,0,0,0};
	int numDigisPerTile=0;
	std::vector<MTTTile*> mttTiles = theGeometry->tiles();
	for (std::vector<MTTTile*>::iterator r = mttTiles.begin(); r != mttTiles.end(); r++) {
		MTTTileId iTile((*r)->id());
		numDigisPerTile=0;
			for(MTTDigiCollection::const_iterator itr=digis->get(iTile).first;itr!=digis->get(iTile).second;++itr){
				numDigisPerTile++;
		  	theHistoMap["DigiRate"]->Fill(iTile.wheel(),iTile.sector());
		  	numDigis[iTile.wheel()+2]++;
		}
		the1dHistoMap["nDigi"]->Fill(numDigisPerTile);
	}
	  theHistoMap["nDigiVsnPU_wm2"]->Fill(numDigis[0],PVI->getPU_NumInteractions());
	  theHistoMap["nDigiVsnPU_wm1"]->Fill(numDigis[1],PVI->getPU_NumInteractions());
	  theHistoMap["nDigiVsnPU_w0"]->Fill(numDigis[2],PVI->getPU_NumInteractions());
	  theHistoMap["nDigiVsnPU_wp1"]->Fill(numDigis[3],PVI->getPU_NumInteractions());
	  theHistoMap["nDigiVsnPU_wp2"]->Fill(numDigis[4],PVI->getPU_NumInteractions());

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
DigiMap::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DigiMap::endJob() 
{
theHistoMap["DigiRate"]->Scale(1/(1.0*totalNumberofEvents)*1/25.0*1/(1e-9));
theHistoMap["DigiRate"]->SetTitle("DIGI-Rate in Hz");
theHistoMap["DigiRate"]->GetXaxis()->SetTitle("Wheel");
theHistoMap["DigiRate"]->GetYaxis()->SetTitle("Sector");

theHistoMap["nDigiVsnPU_wm2"]->GetXaxis()->SetTitle("Number of digis");
theHistoMap["nDigiVsnPU_wm2"]->GetXaxis()->SetTitle("Number of PU vertices");
theHistoMap["nDigiVsnPU_wm2"]->SetTitle("Wheel -2");

theHistoMap["nDigiVsnPU_wm1"]->GetXaxis()->SetTitle("Number of digis");
theHistoMap["nDigiVsnPU_wm1"]->GetXaxis()->SetTitle("Number of PU vertices");
theHistoMap["nDigiVsnPU_wm1"]->SetTitle("Wheel -1");

theHistoMap["nDigiVsnPU_w0"]->GetXaxis()->SetTitle("Number of digis");
theHistoMap["nDigiVsnPU_w0"]->GetXaxis()->SetTitle("Number of PU vertices");
theHistoMap["nDigiVsnPU_w0"]->SetTitle("Wheel 0");

theHistoMap["nDigiVsnPU_wp1"]->GetXaxis()->SetTitle("Number of digis");
theHistoMap["nDigiVsnPU_wp1"]->GetXaxis()->SetTitle("Number of PU vertices");
theHistoMap["nDigiVsnPU_wp1"]->SetTitle("Wheel 1");

theHistoMap["nDigiVsnPU_wp2"]->GetXaxis()->SetTitle("Number of digis");
theHistoMap["nDigiVsnPU_wp2"]->GetXaxis()->SetTitle("Number of PU vertices");
theHistoMap["nDigiVsnPU_wp2"]->SetTitle("Wheel 2");
}

// ------------ method called when starting to processes a run  ------------
void 
DigiMap::beginRun(edm::Run const&, edm::EventSetup const& eventSetup)
{
	edm::ESHandle<MTTGeometry> hGeom;
		eventSetup.get<MuonGeometryRecord> ().get(hGeom);
		theGeometry=&*hGeom;
}

// ------------ method called when ending the processing of a run  ------------
void 
DigiMap::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
DigiMap::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
DigiMap::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DigiMap::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DigiMap);
