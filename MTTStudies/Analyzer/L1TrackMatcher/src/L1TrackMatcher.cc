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
// $Id: L1TrackMatcher.cc,v 1.2 2012/09/03 14:27:45 pmaanen Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "MTTStudies/Geometry/interface/MTTGeometry.h"
#include "MTTStudies/MTTDigi/interface/MTTDigiCollection.h"
#include "SimDataFormats/SLHC/interface/StackedTrackerTypes.h"
#include "SimDataFormats/SLHC/interface/L1TkTrack.h"
#include "SimDataFormats/SLHC/src/L1TkTrack.cc
#include "SimDataFormats/SLHC/src/classes.h"
#include "SimDataFormats/SLHC/src/classes_def.xml"

#include "TrackFinder.h"
//
// class declaration
//

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
L1TrackMatcher::L1TrackMatcher(const edm::ParameterSet& iConfig)
{
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
   std::vector<MTTTile*> TilesWithDigi;
   edm::Handle<MTTDigiCollection> digis;
   iEvent.getByLabel("simMuonMTTDigis",digis);
	MTTDigiCollection::DigiRangeIterator itr;

	edm::Handle<cmsUpgrades::L1Track_PixelDigi_Collection> l1trackHandlePD;
	edm::InputTag l1tracksPixelDigisTag = config.getParameter<edm::InputTag>("L1TracksFromPixelDigis");
	e.getByLabel(l1tracksPixelDigisTag, l1trackHandlePD);

	std::vector<MTTTile*> mttTiles = theGeometry->tiles();
	for (std::vector<MTTTile*>::iterator r = mttTiles.begin(); r != mttTiles.end(); r++) {
		MTTTileId iTile((*r)->id());
		for(MTTDigiCollection::const_iterator itr=digis->get(iTile).first;itr!=digis->get(iTile).second;++itr){
			TilesWithDigi.push_back(*r);
		}
	}
	theTrackFinder.setTiles(&TilesWithDigi);

	/// Go on only if there are L1Tracks from Pixel Digis
	if ( l1trackHandlePD->size() > 0 ) {
		/// Loop over L1Tracks
		L1Track_PixelDigi_Collection::const_iterator iterL1Track;
		for ( iterL1Track = l1trackHandlePD->begin();  iterL1Track != l1trackHandlePD->end();  ++iterL1Track ) {

			/// Select only good L1Tracks
			if (!iterL1Track->isGenuine()) continue;
			/// Get only L1Tracks which are in Muon Barrel Region
			if (abs(iterL1Track->getMomentum.eta())>1.29) continue;

			uint32_t tileid=theTrackFinder.findCrossing(iterL1Track->getMomentum(),iterL1Track->getVertex());
			if(tileid)
				std::cout << MTTTileId(uint32_t) << std::endl;
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
L1TrackMatcher::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1TrackMatcher::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
L1TrackMatcher::beginRun(edm::Run const&, edm::EventSetup const& eventSetup)
{
	edm::ESHandle<MTTGeometry> hGeom;
	eventSetup.get<MuonGeometryRecord> ().get(hGeom);
	theTrackFinder.setGeometry(&*hGeom);
	//theTrackFinder.fillMap();
}

// ------------ method called when ending the processing of a run  ------------
void 
L1TrackMatcher::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
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
