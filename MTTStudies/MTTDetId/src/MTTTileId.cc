/** \file
 * Impl of MTTTileId
 *
 * \author P. Maanen
 * \date 14 Apr 2012
*/


#include <iostream>
#include <MTTStudies/MTTDetId/interface/MTTTileId.h>
#include <FWCore/Utilities/interface/Exception.h>



MTTTileId::MTTTileId() : MTTStripId() {}



MTTTileId::MTTTileId(uint32_t id) {
  id_= id;
  checkMTTId(); // Check this is a valid id for muon DTs.
}



MTTTileId::MTTTileId(int wheel,
		   int station,
		   int sector,
		   int layer,
		   int strip,
		   int tile) : MTTStripId(wheel, station, sector, layer, strip) {
		     if (tile < minTileId || tile > maxTileId) {
		       throw cms::Exception("InvalidDetId") << "MTTTileId ctor:" 
							    << " Invalid parameters: " 
							    << " Wh:"<< wheel
							    << " St:"<< station
							    << " Se:"<< sector
							    << " La:"<< layer
							    << " Str:"<< strip
							    << " Ti:"<< tile
							    << std::endl;
		     }
		     id_ |= (tile & tileMask_) << tileStartBit_;
		   }



// Copy Constructor.
MTTTileId::MTTTileId(const MTTTileId& tileId) {
  id_ = tileId.rawId();
}



// Constructor from a PanelId and layer, strip and tile numbers
MTTTileId::MTTTileId(const MTTPanelId& chId, int layer, int strip, int tile) :
  MTTStripId(chId, layer, strip) {
   if (tile < minTileId || tile > maxTileId) {
      throw cms::Exception("InvalidDetId") << "MTTTileId ctor:" 
					   << " Invalid parameters: " 
					   << " Ti:"<< tile
					   << std::endl;
    }
      
     id_ |= (tile & tileMask_) << tileStartBit_;
  }



// Constructor from a LayerId and strip and tile numbers
MTTTileId::MTTTileId(const MTTLayerId& lId, int strip, int tile) :
  MTTStripId(lId, strip) {
   if (tile < minTileId || tile > maxTileId) {
      throw cms::Exception("InvalidDetId") << "MTTTileId ctor:" 
					   << " Invalid parameters: " 
					   << " Ti:"<< tile
					   << std::endl;
    }
    
    id_ |= (tile & tileMask_) << tileStartBit_;
  }



// Constructor from a stripId and a tile number
MTTTileId::MTTTileId(const MTTStripId& layerId, int tile) : MTTStripId(layerId) {
 if (tile < minTileId || tile > maxTileId) {
      throw cms::Exception("InvalidDetId") << "MTTTileId ctor:" 
					   << " Invalid parameters: " 
					   << " Ti:"<< tile
					   << std::endl;
    }
    
    id_ |= (tile & tileMask_) << tileStartBit_;
}
  


// Ostream operator
std::ostream& operator<<( std::ostream& os, const MTTTileId& id ){

  os << " Wh:"<< id.wheel()
     << " St:"<< id.station() 
     << " Se:"<< id.sector()
     << " La:"<< id.layer()
     << " Str:"<< id.strip()
     << " Ti:"<< id.tile()
     <<" ";

  return os;
}


