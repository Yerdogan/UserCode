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
// $Id$
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
      std::map<std::string,TH1*> theHistoMap;
      uint32_t	totalNumberofEvents;
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
iEvent.getByLabel(,digis);
DigiContainerIterator<MTTTileId,MTTDigi> digiitr;
for(digiitr=digis->begin;digiitr!=digis.end;++digiitr){
HitMap[digiitr->first().rawId()]++;
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

}

// ------------ method called when starting to processes a run  ------------
void 
DigiMap::beginRun(edm::Run const&, edm::EventSetup const&)
{
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
