#include "Geometry/MTTGeometry/interface/MTTStrip.h"
#include "Geometry/MTTGeometry/interface/MTTLayer.h"
#include "Geometry/MTTGeometry/interface/MTTTile.h"

#include <iostream>

MTTStrip::MTTStrip(MTTStripId id, ReferenceCountingPointer<BoundPlane>& plane, const MTTLayer* ly) :
	GeomDet(plane), theId(id), theTiles(24, (const MTTTile*) 0), theLayer(ly) {
	setDetId(id);
}

MTTStrip::~MTTStrip() {
	for (std::vector<const MTTTile*>::const_iterator itile = theTiles.begin(); itile != theTiles.end(); ++itile)
		delete (*itile);
}

MTTStripId MTTStrip::id() const {
	return theId;
}

bool MTTStrip::operator==(const MTTStrip& st) const {
	return id() == st.id();
}

/// Return the tiles in the strip
std::vector<const GeomDet*> MTTStrip::components() const {
	return std::vector<const GeomDet*>(theTiles.begin(), theTiles.end());
}

const GeomDet* MTTStrip::component(DetId id) const {
	return tile(MTTTileId(id.rawId()));
}

const std::vector<const MTTTile*>& MTTStrip::tiles() const {
	return theTiles;
}

void MTTStrip::add(MTTTile* t) {
	// theTiles size is preallocated (24) - change if geometry changes.
	theTiles[t->id().tile() - 1] = t;
}

const MTTPanel* MTTStrip::panel() const {
	return (theLayer) ? theLayer->panel() : 0;
}

const MTTLayer* MTTStrip::layer() const {
	return theLayer;
}

const MTTTile* MTTStrip::tile(MTTTileId id) const {
	if (id.stripId() != theId)
		return 0;
	return tile(id.tile());
}

const MTTTile* MTTStrip::tile(int itile) const {
	if ((itile >= 1) && (itile <= 24)) {
		return theTiles[itile - 1];
	} else {
		return 0;
	}
}
