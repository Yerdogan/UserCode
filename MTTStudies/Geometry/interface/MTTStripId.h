#ifndef MuonDetId_MTTStripId_h
#define MuonDetId_MTTStripId_h

/** \class MTTStripId
 *  DetUnit identifier for MTT strips
 *
 *  $Date: 2012/07/19 14:18:01 $
 *  $Revision: 1.1 $
 *  \author G. Cerminara - INFN Torino
 */

#include <MTTStudies/Geometry/interface/MTTLayerId.h>

class MTTStripId : public MTTLayerId {
 public:
      
  /// Default constructor. 
  /// Fills the common part in the base and leaves 0 in all other fields
  MTTStripId();


  /// Construct from a packed id.   
  /// It is required that the packed id represents a valid DT DetId
  /// (proper Detector and  SubDet fields), otherwise an exception is thrown.
  /// Any bits outside the MTTStripId fields are zeroed; apart for
  /// this, no check is done on the vaildity of the values.
  explicit MTTStripId(uint32_t id);


  /// Construct from indexes.
  /// Input values are required to be within legal ranges, otherwise an
  /// exception is thrown.
  MTTStripId(int wheel, 
	    int station, 
	    int sector,
	    int layer,
	    int strip);


  /// Copy Constructor.
  /// Any bits outside the MTTStripId fields are zeroed; apart for
  /// this, no check is done on the vaildity of the values.
  MTTStripId(const MTTStripId& stripId);


  /// Constructor from a camberId and layer and strip numbers
  MTTStripId(const MTTPanelId& chId, int layer, int strip);


  /// Constructor from a LayerId and strip number
  MTTStripId(const MTTLayerId& lId, int strip);


  /// Return the strip number
  int strip() const {
    return ((id_>>stripStartBit_)&stripMask_);
  }


  /// Return the corresponding SuperLayerId
  MTTLayerId layerId() const {
    return MTTLayerId(id_ & layerIdMask_);
  }
  


};


std::ostream& operator<<( std::ostream& os, const MTTStripId& id );

#endif
