/* This Class Header */
#include "Geometry/MTTGeometry/interface/MTTTileType.h"

/* Collaborating Class Header */
#include "Geometry/MTTGeometry/interface/MTTTopology.h"

/* Constructor */
MTTTileType::MTTTileType() :
	GeomDetType("MTT", GeomDetEnumerators::MTT) {
}

/* Operations */
const Topology& MTTTileType::topology() const {
	static MTTTopology result(0, 0, 0.);
	return result;
}

