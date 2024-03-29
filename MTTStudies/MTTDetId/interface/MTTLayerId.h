#ifndef MuonDetId_MTTLayerId_H
#define MuonDetId_MTTLayerId_H

/** \class MTTLayerId
 *  DetUnit identifier for MTT Layers
 *
 *  $Date: 2012/07/23 12:48:43 $
 *  $Revision: 1.1 $
 *  \author P. Maanen
 */

#include <MTTStudies/MTTDetId/interface/MTTPanelId.h>


class MTTLayerId : public MTTPanelId {
public:

  /// Default constructor. It fills the common part in the base
  /// and leaves 0 in all other fields
  MTTLayerId();


  /// Construct from a packed id.
  /// It is required that the packed id represents a valid MTT DetId
  /// (proper Detector and  SubDet fields), otherwise an exception is thrown.
  /// Any bits outside the MTTLayerId fields are zeroed; apart for
  /// this, no check is done on the validity of the values.
  explicit MTTLayerId(uint32_t id);


  /// Construct from indexes.
  /// Input values are required to be within legal ranges, otherwise an
  /// exception is thrown.
  MTTLayerId(int wheel, 
		 int station, 
		 int sector,
		 int layer);


  /// Copy Constructor.
  /// Any bits outside the MTTPanelId fields are zeroed; apart for
  /// this, no check is done on the vaildity of the values.
  MTTLayerId(const MTTLayerId& lId);


  /// Constructor from a MTTPanelId and layer number.
  MTTLayerId(const MTTPanelId& chId, int layer);


  /// Return the layer number
  int layer() const {
    return ((id_>>layerStartBit_)&layerMask_);
  }

  /// Return the corresponding PanelId
  MTTPanelId panelId() const {
    return MTTPanelId(id_ & PanelIdMask_);
  }

 private:

};

std::ostream& operator<<( std::ostream& os, const MTTLayerId& id );

#endif

