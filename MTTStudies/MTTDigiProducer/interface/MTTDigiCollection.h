#ifndef MTTDigi_MTTDigiCollection_h
#define MTTDigi_MTTDigiCollection_h

#include <DataFormats/MuonDetId/interface/MTTTileId.h>
#include <DataFormats/MTTDigi/interface/MTTDigi.h>
#include <DataFormats/MuonData/interface/MuonDigiCollection.h>

typedef MuonDigiCollection<MTTTileId, MTTDigi> MTTDigiCollection;

#endif