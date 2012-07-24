#ifndef MTTDigi_MTTDigiCollection_h
#define MTTDigi_MTTDigiCollection_h

#include <MTTStudies/MTTDetId/interface/MTTTileId.h>
#include <MTTStudies/MTTDigiProducer/interface/MTTDigi.h>
#include <DataFormats/MuonData/interface/MuonDigiCollection.h>

typedef MuonDigiCollection<MTTTileId, MTTDigi> MTTDigiCollection;

#endif
