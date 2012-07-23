/** \file
 * Impl of MTTStripId
 *
 * \author P. Maanen
 * \version $Id: MTTStripId.cc,v 1.2 2012/07/19 14:49:30 pmaanen Exp $
 * \date 12 Apr 2012
*/

#include <iostream>
#include <MTTStudies/Geometry/interface/MTTStripId.h>
#include <FWCore/Utilities/interface/Exception.h>



MTTStripId::MTTStripId() : MTTLayerId() {}


MTTStripId::MTTStripId(uint32_t id) {
  // Mask the bits outside MTTStripId fields
  id_ = id & stripIdMask_;
  // Check this is a valid id for MTT.
  checkMTTId();
}



// Copy Constructor.
MTTStripId::MTTStripId(const MTTStripId& stripId) {
  // The mask is required for proper slicing, i.e. if stripId is
  // actually a derived class.
  id_ = (stripId.rawId() & stripIdMask_);
}



// Constructor from a camberId and layer and strip numbers
MTTStripId::MTTStripId(const MTTPanelId& chId, int layer, int strip) : MTTLayerId(chId, layer) {
  if (strip< minStripId || strip > maxStripId) {
    throw cms::Exception("InvalidDetId") << "MTTStripId ctor:" 
					 << " Invalid parameters: " 
					 << " Str:"<< strip
					 << std::endl;
  }
  id_ |= (strip & stripMask_) << stripStartBit_;
}



// Constructor from a LayerId and strip number
MTTStripId::MTTStripId(const MTTLayerId& slId, int strip) : MTTLayerId(slId) {
  if (strip< minStripId || strip > maxStripId) {
    throw cms::Exception("InvalidDetId") << "MTTStripId ctor:" 
					 << " Invalid parameters: " 
					 << " Str:"<< strip
					 << std::endl;
  }
  id_ |= (strip & stripMask_) << stripStartBit_;
}




MTTStripId::MTTStripId(int wheel,
		     int station,
		     int sector,
		     int layer,
		     int strip) : MTTLayerId(wheel, station, sector, layer) {
		        if (strip< minStripId || strip > maxStripId) {
			 throw cms::Exception("InvalidDetId") << "MTTStripId ctor:" 
							      << " Invalid parameters: " 
							      << " Wh:"<< wheel
							      << " St:"<< station
							      << " Se:"<< sector
							      << " La:"<< layer
							      << " Str:"<< strip
							      << std::endl;
		       }
		       
		       id_ |= (strip & stripMask_) << stripStartBit_;
		     }



std::ostream& operator<<( std::ostream& os, const MTTStripId& id ){
  os << " Wh:"<< id.wheel()
     << " St:"<< id.station() 
     << " Se:"<< id.sector()
     << " La:"<< id.layer()
     << " Str:"<< id.strip()
     <<" ";

  return os;
}


