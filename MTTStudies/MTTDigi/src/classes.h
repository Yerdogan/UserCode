#include <MTTStudies/MTTDigi/interface/MTTDigi.h>
#include <MTTStudies/MTTDigi/interface/MTTDigiCollection.h>
#include <MTTStudies/MTTDetId/interface/MTTTileId.h>
#include <MTTStudies/MTTDigi/interface/MTTDigiSimLink.h>

#include <DataFormats/Common/interface/Wrapper.h>
#include "DataFormats/Common/interface/DetSetVector.h"
#include <vector>
#include <map>

namespace{ 
  struct dictionary {
    
    MTTDigi d;
    MTTDigiSimLink dsl;
    std::vector<MTTDigi>  vv;
    std::vector<std::vector<MTTDigi> >  v1; 
    std::vector<edm::DetSet<MTTDigiSimLink> > v2;
    std::vector<MTTDigiSimLink> v3;
    MTTDigiCollection dd;
    edm::DetSet<MTTDigiSimLink> disisimlink_ds;
    edm::DetSetVector<MTTDigiSimLink> digisimlink_dsv;
    
    edm::Wrapper<MTTDigiCollection> dw;
    edm::Wrapper<std::map< std::pair<int,int>, int > > a2;
    edm::Wrapper<MuonDigiCollection<MTTTileId,MTTDigi> > muondigiwr;
    edm::Wrapper<edm::DetSetVector<MTTDigiSimLink> > digisimwr;

  };
}
