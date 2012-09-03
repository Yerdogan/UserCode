#include "MTTStudies/Geometry/interface/MTTNumberingScheme.h"
#include "MTTStudies/MTTDetId/interface/MTTTileId.h"
#include "Geometry/MuonNumbering/interface/MuonBaseNumber.h"
#include "Geometry/MuonNumbering/interface/MuonDDDConstants.h"
#include <iostream>

//#define LOCAL_DEBUG

MTTNumberingScheme::MTTNumberingScheme( const MuonDDDConstants& muonConstants ) {
  initMe(muonConstants);
}

MTTNumberingScheme::MTTNumberingScheme( const DDCompactView& cpv ) {
  MuonDDDConstants muonConstants(cpv);
  initMe(muonConstants);
}

void MTTNumberingScheme::initMe ( const MuonDDDConstants& muonConstants ) {
  int theLevelPart=muonConstants.getValue("level");
  theRegionLevel=muonConstants.getValue("mb_region")/theLevelPart;
  theWheelLevel=muonConstants.getValue("mb_wheel")/theLevelPart;
  theStationLevel=muonConstants.getValue("mtt_panel")/theLevelPart;
  theLayerLevel=muonConstants.getValue("mtt_layer")/theLevelPart;
  theStripLevel=muonConstants.getValue("mtt_strip")/theLevelPart;
  theTileLevel=muonConstants.getValue("mtt_tile")/theLevelPart;
 #ifdef LOCAL_DEBUG
   std::cout << "theRegionLevel " << theRegionLevel <<std::endl;
   std::cout << "theWheelLevel " << theWheelLevel <<std::endl;
   std::cout << "theStationLevel " << theStationLevel <<std::endl;
   std::cout << "theLayerLevel " << theLayerLevel <<std::endl;
   std::cout << "theStripLevel " << theStripLevel <<std::endl;
   std::cout << "theTileLevel " << theTileLevel <<std::endl;
 #endif

}

int MTTNumberingScheme::baseNumberToUnitNumber(const MuonBaseNumber num){
  
#ifdef LOCAL_DEBUG
  std::cout << "MTTNumbering "<<num.getLevels()<<std::endl;
  for (int level=1;level<=num.getLevels();level++) {
    std::cout << level << " " << num.getSuperNo(level)
       << " " << num.getBaseNo(level) << std::endl;
  }
#endif
  if (num.getLevels()!=theTileLevel) {
    std::cout << "BaseNumber has " << num.getLevels() << " levels,"
       << "need "<<theTileLevel<<std::endl;
    return 0;
  }
  return getDetId(num);
}

uint32_t MTTNumberingScheme::getDetId(const MuonBaseNumber num) const {
  
  int tile_id=0;
  int strip_id=0;
  int layer_id=0;
  int sector_id=0;
  int station_id=0;
  int wheel_id=0;

  //decode significant barrel levels
  decode(num,
         tile_id,
         strip_id,
         layer_id,
         sector_id,
         station_id,
         wheel_id);
  MTTTileId id(wheel_id,station_id,sector_id,layer_id,strip_id,tile_id);
  
#ifdef LOCAL_DEBUG
  std::cout << "MTTNumberingScheme: " << id << std::endl;
#endif
  
  return id.rawId();
}

void MTTNumberingScheme::decode(const MuonBaseNumber& num,
                                       int& tile_id,
                                       int& strip_id,
                                       int& layer_id,
                                       int& sector_id,
                                       int& station_id,
                                       int& wheel_id) const {

  for (int level=1;level<=num.getLevels();level++) {

    //decode
    if (level==theWheelLevel) {
      const int copyno=num.getBaseNo(level);      
      wheel_id=copyno-2;

    } else if (level==theStationLevel) {
      const int station_tag = num.getSuperNo(level);
      const int copyno = num.getBaseNo(level);
      station_id=station_tag;
      sector_id=copyno+1;   

    } else if (level==theLayerLevel) {
      const int copyno = num.getBaseNo(level);
      layer_id = copyno + 1;

    } else if (level==theStripLevel) {
      const int copyno = num.getBaseNo(level);
      strip_id=copyno+1;

    } else if (level==theTileLevel) {
      const int copyno = num.getBaseNo(level);
      tile_id = copyno+1;
    }
  }
}
