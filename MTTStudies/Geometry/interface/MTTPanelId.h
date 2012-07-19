#ifndef MuonDetId_MTTPanelId_H
#define MuonDetId_MTTPanelId_H

/** \class MTTPanelId
 *  DetUnit identifier for MTT Panels.
 *  
 *  $Date: 2012/05/03 12:28:08 $
 *  $Revision: 1.1 $
 *  \author Paul Maanen
 */

#include <DataFormats/DetId/interface/DetId.h>

#include <iosfwd>

class MTTPanelId : public DetId {
public:
  /// Default constructor. 
  /// Fills the common part in the base and leaves 0 in all other fields
  MTTPanelId();
  

  /// Construct from a packed id.
  /// It is required that the packed id represents a valid MTT DetId
  /// (proper Detector and  SubDet fields), otherwise an exception is thrown.
  /// Any bits outside the DTPanelId fields are zeroed; apart for
  /// this, no check is done on the vaildity of the values.
  MTTPanelId(uint32_t id);
  MTTPanelId(DetId id);


  /// Construct from indexes.
  /// Input values are required to be within legal ranges, otherwise an
  /// exception is thrown.
  MTTPanelId(int wheel,
	      int station, 
	      int sector);


  /// Copy Constructor.
  /// Any bits outside the DTPanelId fields are zeroed; apart for
  /// this, no check is done on the vaildity of the values.
  MTTPanelId(const MTTPanelId& chId);


  /// Return the wheel number
  int wheel() const {
    return int((id_>>wheelStartBit_) & wheelMask_)+ minWheelId -1;
  }


  /// Return the station number
  int station() const {
    return ((id_>>stationStartBit_) & stationMask_);
  }


  /// Return the sector number. Sectors are numbered from 1 to 12,
  /// starting at phi=0 and increasing with phi.
  /// In station 4, where the top and bottom setcors are made of two Panels,
  /// two additional sector numbers are used, 13 (after sector 4, top)
  /// and 14 (after sector 10, bottom).
  int sector() const {
    return ((id_>>sectorStartBit_)& sectorMask_);
  }


  /// lowest station id
  static const int minStationId=    1;
  /// highest station id
  static const int maxStationId=    4;
  /// lowest sector id. 0 indicates all sectors (a station) 
  static const int minSectorId=     0;
  /// highest sector id.
  static const int maxSectorId=    12;
  /// lowest wheel number
  static const int minWheelId=     -2;
  /// highest wheel number
  static const int maxWheelId=      2;
  /// lowest layer id. 0 indicates a full Panel
  static const int minLayerId=      0;
  /// highest layer id
  static const int maxLayerId=      4;
  ///lowest strip id. 0 indicated a full layer
  static const int minStripId=   0;
  ///highest strip id
  static const int maxStripId=18  ;
  /// lowest tile id (numbering starts from 1). 0 indicates a full layer
  static const int minTileId=       0;
  /// highest wire id (Panels have 48 to 96 wires)
  static const int maxTileId=      25;
 

 protected:
  /// two bits would be enough, but  we could use the number "0" as a wildcard
  static const int tileNumBits_=   5;  
  static const int tileStartBit_=  2;
  static const int stripNumBits_=   5;
  static const int stripStartBit_=  tileStartBit_ + tileNumBits_;
  static const int layerNumBits_=  3;
  static const int layerStartBit_= stripStartBit_+ stripNumBits_;
  static const int wheelNumBits_  = 3;
  static const int wheelStartBit_=  layerStartBit_ + layerNumBits_;
  static const int sectorNumBits_=  4;
  static const int sectorStartBit_= wheelStartBit_ + wheelNumBits_;
  /// two bits would be enough, but  we could use the number "0" as a wildcard
  static const int stationNumBits_= 3;
  static const int stationStartBit_ = sectorStartBit_ + sectorNumBits_;


  static const uint32_t wheelMask_=    0x7;
  static const uint32_t stationMask_=  0x7;
  static const uint32_t sectorMask_=   0xf;
  static const uint32_t layerMask_=    0x7;
  static const uint32_t stripMask_=   0x1f;
  static const uint32_t tileMask_=    0x1f;

  static const uint32_t stripIdMask_= ~(tileMask_<<tileStartBit_);
  static const uint32_t layerIdMask_ = ~((tileMask_<<tileStartBit_) |
				      (stripMask_<<stripStartBit_));
  static const uint32_t PanelIdMask_ = ~((tileMask_<<tileStartBit_) |
					   (stripMask_<<stripStartBit_) |
					   (layerMask_<<layerStartBit_));

  // Perform a consistency check of the id with a DT Id
  // It throws an exception if this is not the case
  void checkMTTId();
};


std::ostream& operator<<( std::ostream& os, const MTTPanelId& id );

#endif
