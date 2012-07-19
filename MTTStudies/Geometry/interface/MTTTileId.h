#ifndef MuonDetId_MTTTileId_h
#define MuonDetId_MTTTileId_h

/** \class MTTTileId
 *  DetUnit identifier for MTT Tiles
 *
 *  $Date: 2012/05/03 12:28:08 $
 *  $Revision: 1.2 $
 *  \author P. Maanen
 */

#include <DataFormats/MuonDetId/interface/MTTStripId.h>


class MTTTileId :public MTTStripId {
 public:
      
  /// Default constructor.
  /// Fills the common part in the base and leaves 0 in all other fields
  MTTTileId();


  /// Construct from a packed id. 
  /// It is required that the packed id represents a valid DT DetId
  /// (proper Detector and  SubDet fields), otherwise an exception is thrown.
  /// No check is done on the vaildity of the values.
  explicit MTTTileId(uint32_t id);


  /// Construct from fully qualified identifier. 
  /// Input values are required to be within legal ranges, otherwise an
  /// exception is thrown.
  MTTTileId(int wheel, 
	   int station, 
	   int sector,
	   int layer,
	   int strip,
	   int tile);
  

  /// Copy Constructor.
  MTTTileId(const MTTTileId& wireId);


  /// Constructor from a PanelId and layer and strip and tile numbers
  MTTTileId(const MTTPanelId& chId, int layer, int strip, int tile);


  /// Constructor from a LayerId and strip and tile numbers
  MTTTileId(const MTTLayerId& lId, int layer, int tile);


  /// Constructor from a layerId and a wire number
  MTTTileId(const MTTStripId& layerId, int tile);


  /// Return the tile number
  int tile() const {
    return ((id_>>tileStartBit_)&tileMask_);
  }


  /// Return the corresponding LayerId
  MTTStripId stripId() const {
    return MTTStripId(id_ & stripIdMask_);
  }
  

 private:
 

};

std::ostream& operator<<( std::ostream& os, const MTTTileId& id );

#endif
