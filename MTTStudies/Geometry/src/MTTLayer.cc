#include "Geometry/MTTGeometry/interface/MTTLayer.h"

/* Collaborating Class Header */
#include "Geometry/MTTGeometry/interface/MTTStrip.h"
#include "Geometry/MTTGeometry/interface/MTTTile.h"

/* C++ Headers */
#include <iostream>

/* ====================================================================== */

/* Constructor */
MTTLayer::MTTLayer(MTTLayerId id, const ReferenceCountingPointer<BoundPlane>& plane, MTTPanel* pa) :
	GeomDet(plane), theId(id), thePanel(pa) {
	setDetId(id);
}

/* Destructor */
MTTLayer::~MTTLayer() {
	for (std::vector<const MTTStrip*>::const_iterator ist = theStrips.begin(); ist != theStrips.end(); ++ist)
		delete (*ist);
}

/* Operations */

MTTLayerId MTTLayer::id() const {
	return theId;
}

bool MTTLayer::operator==(const MTTLayer& ly) const {
	return id() == ly.id();
}

void MTTLayer::add(MTTStrip* st) {
	theStrips.push_back(st);
}

std::vector<const GeomDet*> MTTLayer::components() const {
	return std::vector<const GeomDet*>(theStrips.begin(), theStrips.end());
}

const GeomDet* MTTLayer::component(DetId id) const {
	// return layer components with a given id, distinguish between strip or tile
	MTTTileId tId(id.rawId());
	if (tId.tile() == 0) { // is a strip id
		return strip(tId);
	} else { // is a tile id
		return tile(tId);
	}
}

const std::vector<const MTTStrip*>& MTTLayer::strips() const {
	return theStrips;
}

const MTTStrip* MTTLayer::strip(MTTStripId id) const {
	if (id.panelId() != theId)
		return 0; // FIXME not in this strip!
	return strip(id.strip());
}

const MTTStrip* MTTLayer::strip(int ist) const {
	for (std::vector<const MTTStrip*>::const_iterator i = theStrips.begin(); i != theStrips.end(); ++i) {
		if ((*i)->id().strip() == ist)
			return (*i);
	}
	return 0;
}

const MTTTile* MTTLayer::tile(MTTTileId id) const {
	return (strip(id.strip()))->tile(id.tile());
}

const MTTPanel* MTTLayer::panel() const {
	return thePanel;
}
