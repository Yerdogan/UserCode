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
#include "FastSimulation/Particle/interface/RawParticle.h"
#include "FastSimulation/BaseParticlePropagator/interface/BaseParticlePropagator.h"

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

using namespace std;
using namespace edm;
using namespace reco;
using namespace cmsUpgrades;
using namespace l1slhc;
using namespace l1extra;

#include "TrackFinder.h"
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
	l1TrackInputTag=iConfig.getParameter<edm::InputTag>("l1TrackSource");
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

	edm::Handle<L1TkTrack_PixelDigi_Collection> l1trackHandlePD;

	iEvent.getByLabel(l1TrackInputTag, l1trackHandlePD);

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
		L1TkTrack_PixelDigi_Collection::const_iterator iterL1Track;
		for ( iterL1Track = l1trackHandlePD->begin();  iterL1Track != l1trackHandlePD->end();  ++iterL1Track ) {

			/// Select only good L1Tracks
			if (!iterL1Track->isGenuine()) continue;
			/// Get only L1Tracks which are in Muon Barrel Region
			if (abs(iterL1Track->getMomentum().eta())>1.29) continue;

			uint32_t tileid=theTrackFinder.findCrossing(iterL1Track->getMomentum(),iterL1Track->getVertex());
			if(tileid)
				std::cout << MTTTileId(tileid) << std::endl;
		}
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
	theGeometry=&*hGeom;
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
