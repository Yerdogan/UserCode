#ifndef MTTTILETYPE_H
#define MTTTILETYPE_H

/** \class MTTileType
 *
 *  DetType for a MTT Tile GeomDetUnit (the MTTTile).
 *
 */

/* Base Class Headers */
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"

class MTTTileType: public GeomDetType {

public:

	/* Constructor */
	MTTTileType();

	/* Operations */
	virtual const Topology& topology() const;

private:

};
#endif // MTTTILETYPE_H
