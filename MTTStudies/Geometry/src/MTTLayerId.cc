/** \file
 *  See header file for a description of this class.
 *
 *  $Date: 2012/05/03 12:28:09 $
 *  $Revision: 1.3 $
 *  \author P. Maanen
 */

#include <iostream>
#include <DataFormats/MuonDetId/interface/MTTLayerId.h>
#include <FWCore/Utilities/interface/Exception.h>



MTTLayerId::MTTLayerId() : MTTPanelId() {}



MTTLayerId::MTTLayerId(uint32_t id) {
  id_ = id & layerIdMask_; // Mask the bits outside MTTLayerId fields
  checkMTTId();        // Check this is a valid id for MTT.
}


MTTLayerId::MTTLayerId(int wheel,
	       int station,
	       int sector,
	       int layer) : MTTPanelId(wheel, station, sector) {
		 if(layer < minLayerId || layer > maxLayerId) {
		   throw cms::Exception("InvalidDetId") << "MTTLayerId ctor:" 
							<< " Invalid parameters: " 
							<< " Wh:"<< wheel
							<< " St:"<< station
							<< " Se:"<< sector
							<< " La:"<< layer
							<< std::endl;
		 }
		 id_ |= (layer & layerMask_) << layerStartBit_;
	       }



// Copy Constructor.
MTTLayerId::MTTLayerId(const MTTLayerId& lId) {
  // The mask is required for proper slicing, i.e. if slId is
  // actually a derived class.
  id_ = (lId.rawId() &  layerIdMask_);						 
}



// Constructor from a camberId and l number
MTTLayerId::MTTLayerId(const MTTPanelId& chId, int layer) : MTTPanelId(chId) {
  if(layer < minLayerId || layer > maxLayerId) {
    throw cms::Exception("InvalidDetId") << "MTTLayerId ctor:" 
					 << " Invalid parameter: " 
					 << " La:"<< layer
					 << std::endl;
  }
  id_ |= (layer & layerMask_) << layerStartBit_;
}






std::ostream& operator<<( std::ostream& os, const MTTLayerId& id ){
  os << " Wh:"<< id.wheel()
     << " St:"<< id.station() 
     << " Se:"<< id.sector()
     << " Sl:"<< id.layer()
     <<" ";

  return os;
}

