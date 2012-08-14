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
// $Id: DigiMap.cc,v 1.1 2012/08/10 15:44:52 pmaanen Exp $
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
	//TODO: find out name for digis
	iEvent.getByLabel("simMuonMTTDigis",digis);
	MTTDigiCollection::DigiRangeIterator itr;
	std::cout<<"got Digis"<<std::endl;
	int numDigis=0;
	std::vector<MTTTile*> mttTiles = theGeometry->tiles();
	//std::vector<MTTDigi>::const_iterator itr;
	for (std::vector<MTTTile*>::iterator r = mttTiles.begin(); r != mttTiles.end(); r++) {
		MTTTileId iTile((*r)->id());
                        std::cout<<"I am in wheel: "<<iTile.wheel()<<" sector: "<<iTile.sector()<<std::endl;
		if(digis->get(iTile).first!=digis->get(iTile).second)
			std::cout<<"Digis in Wheel: "<<iTile.wheel()<<" Sector: "<<iTile.sector()<<std::endl;
	}

	for(itr=digis->begin();itr!=digis->end();++itr){
		numDigis=0;
		MTTTileId Tile((*itr).first.rawId());
		std::cout<<"Wheel: "<<Tile.wheel()<<" Sector: "<<Tile.sector()<<" Count: "<<numDigis<<std::endl;
		for ( std::vector<MTTDigi>::const_iterator digiIter = (*itr).second.first;
				digiIter != (*itr).second.second; digiIter++) numDigis++ ;
		HitMap[(*itr).first.rawId()]=numDigis;
	}
	for(std::map<uint32_t,uint32_t>::iterator iTile=HitMap.begin();iTile!=HitMap.end();++iTile){
		//theHistoMap["DigiRate"]->Fill((double)Tile.wheel(),(double)Tile.sector(),(double)iTile->second);
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
void 
DigiMap::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DigiMap::endJob() 
{
theHistoMap["DigiRate"]->Scale(1/totalNumberofEvents*1/25*1/(1e-9));
theHistoMap["DigiRate"]->SetTitle("DIGI-Rate in Hz");
theHistoMap["DigiRate"]->GetXaxis()->SetTitle("Wheel");
theHistoMap["DigiRate"]->GetYaxis()->SetTitle("Sector");
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
