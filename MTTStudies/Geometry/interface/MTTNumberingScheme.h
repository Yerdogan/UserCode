#ifndef MuonNumbering_MTTNumberingScheme_h
#define MuonNumbering_MTTNumberingScheme_h

/** \class MTTNumberingScheme
 *
 * implementation of MuonNumberingScheme for muon barrel,
 * converts the MuonBaseNumber to a unit id
 *  
 */

#include "Geometry/MuonNumbering/interface/MuonNumberingScheme.h"

class MuonBaseNumber;
class MuBarDetBuilder;
class DDCompactView;
class MuonDDDConstants;

class MTTNumberingScheme : public MuonNumberingScheme {
 public:

  MTTNumberingScheme( const DDCompactView& cpv );
  MTTNumberingScheme( const MuonDDDConstants& muonConstants );
  ~MTTNumberingScheme(){}
  
  virtual int baseNumberToUnitNumber(const MuonBaseNumber num);

  int getDetId(const MuonBaseNumber num) const;
  
 private:

  void initMe ( const MuonDDDConstants& muonConstants );
  // Decode MuonBaseNumber to id: no checking
  void decode(const MuonBaseNumber& num,
              int& tile_id,
              int& strip_id,
              int& layer_id,
              int& sector_id,
              int& station_id,
              int& wheel_id
             ) const;

  int theRegionLevel;
  int theWheelLevel;
  int theStationLevel;
  int theLayerLevel;
  int theStripLevel;
  int theTileLevel;
};

#endif
