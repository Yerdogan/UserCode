#ifndef MTTDigi_MTTDigi_h
#define MTTDigi_MTTDigi_h

#include <boost/cstdint.hpp>
#include <iosfwd>

class MTTDigi{

public:
  explicit MTTDigi (int tile);
  MTTDigi ();

  bool operator==(const MTTDigi& digi) const;
//  bool operator<(const MTTDigi& digi) const;

  int tile() const ;
  void print() const;

private:
  uint16_t tile_;
};

std::ostream & operator<<(std::ostream & o, const MTTDigi& digi);

#endif