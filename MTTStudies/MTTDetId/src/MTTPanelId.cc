/** \file
 *  See header file for a description of this class.
 *
 *  $Date: 2012/07/23 12:48:44 $
 *  $Revision: 1.1 $
 *  \author P. Maanen
 */

#include "MTTStudies/MTTDetId/interface/MTTPanelId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h" 
#include "FWCore/Utilities/interface/Exception.h"
#include <ostream>

using namespace std;

MTTPanelId::MTTPanelId() : DetId(DetId::Muon, 4){}
//We use 4 instead as SubDetId for MTT 

MTTPanelId::MTTPanelId(uint32_t id) :
  DetId(id & PanelIdMask_) { // Mask the bits outside DTPanelId fields
  checkMTTId();               // Check this is a valid id for muon DTs.
}
MTTPanelId::MTTPanelId(DetId id) :
  DetId(id.rawId() & PanelIdMask_) { // Mask the bits outside MTTPanelId fields
  checkMTTId();               // Check this is a valid id for muon MTTs.
}



MTTPanelId::MTTPanelId(int wheel, int station, int sector):
  DetId(DetId::Muon, 4) {
    // Check that arguments are within the range
    if (wheel      < minWheelId      || wheel      > maxWheelId ||
	station    < minStationId    || station    > maxStationId ||
	sector     < minSectorId     || sector     > maxSectorId) {
      throw cms::Exception("InvalidDetId") << "MTTPanelId ctor:"
					   << " Invalid parameters: " 
					   << " Wh:"<< wheel
					   << " St:"<< station
					   << " Se:"<< sector
					   << std::endl;
    }

    int tmpwheelid = wheel- minWheelId +1;
    id_ |= (tmpwheelid& wheelMask_) << wheelStartBit_   |
      (station & stationMask_)      << stationStartBit_ |
      (sector  &sectorMask_ )       << sectorStartBit_;

}



MTTPanelId::MTTPanelId(const MTTPanelId& chId) :
  DetId(chId.rawId() & PanelIdMask_) {   // The mask is required for proper slicing, i.e. if chId is actually a derived class.
}



void MTTPanelId::checkMTTId() {
  if (det()!=DetId::Muon || subdetId()!=4) {
    throw cms::Exception("InvalidDetId") << "MTTPanelId ctor:"
					 << " det: " << det()
					 << " subdet: " << subdetId()
					 << " is not a valid MTT id";  
  }
}



std::ostream& operator<<( std::ostream& os, const MTTPanelId& id ){
  os << " Wh:"<< id.wheel()
     << " St:"<< id.station() 
     << " Se:"<< id.sector()
     <<" ";

  return os;
}



