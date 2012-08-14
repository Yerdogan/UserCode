#include "MTTStudies/MTTDigi/interface/MTTDigi.h"
#include <iostream>

MTTDigi::MTTDigi (int tile) :
  tile_(tile)
{}

MTTDigi::MTTDigi ():
  tile_(0)
{}


// Comparison
bool
MTTDigi::operator == (const MTTDigi& digi) const {
  if ( tile_ != digi.tile() ) return false;
  return true;
}

/////Precedence operator
//bool 
//MTTDigi::operator<(const MTTDigi& digi) const{
//
//  if(digi.bx() == this->bx())
//    return digi.strip()<this->strip();
//  else 
//    return digi.bx()<this->bx();
//}

std::ostream & operator<<(std::ostream & o, const MTTDigi& digi) {
  return o << " " << digi.tile();
}

int MTTDigi::tile() const { return tile_; }

void
MTTDigi::print() const {
  std::cout << "Tile " << tile() << std::endl;
}
