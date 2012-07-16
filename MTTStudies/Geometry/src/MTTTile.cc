#include "Geometry/MTTGeometry/interface/MTTTile.h"

#include "Geometry/MTTGeometry/interface/MTTStrip.h"

MTTTile::MTTTile(MTTTileId id, ReferenceCountingPointer<BoundPlane>& plane, const MTTStrip* st) :
	GeomDetUnit(*&plane), theId(id), theStrip(st), theTopo(0, 0, 0) {
	setDetId(id);
}

MTTTile::~MTTTile() {

}

MTTTileId MTTTile::id() const {
	return theId;
}

bool MTTTile::operator==(const MTTTile& t) const {
	return id() == t.id();
}

const MTTStrip* MTTTile::strip() const {
	return theStrip;
}

const MTTLayer* MTTTile::layer() const {
	return (theStrip) ? theStrip->layer() : 0;
}

const MTTPanel* MTTTile::panel() const {
	return (theStrip) ? theStrip->panel() : 0;
}

std::vector<const GeomDet*> MTTTile::components() const {
	return std::vector<const GeomDet*>();
}

const GeomDetType& MTTTile::type() const {
	return theType;
}

const Topology& MTTTile::topology() const {
	return theTopo;
}
