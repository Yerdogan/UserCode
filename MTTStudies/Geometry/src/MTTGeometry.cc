
#include <Geometry/MTTGeometry/interface/MTTGeometry.h>
#include <Geometry/CommonDetUnit/interface/GeomDetUnit.h>

#include <algorithm>
#include <iostream>

MTTGeometry::MTTGeometry() {
}

MTTGeometry::~MTTGeometry() {
	// delete all the layers (which will delete the St which will delete the
	// tiles)
	for (std::vector<MTTLayer*>::const_iterator ily = theLayers.begin(); ily != theLayers.end(); ++ily)
		delete (*ily);
}

const MTTGeometry::DetTypeContainer& MTTGeometry::detTypes() const {
	static DetTypeContainer theDetTypes;
	// FIXME - fill it at runtime
	return theDetTypes;
}

void MTTGeometry::add(MTTPanel* pa) {
	// add panels to the dets, Panels and Map
	theDets.push_back(pa);
	thePanels.push_back(pa);
	theMap.insert(MTTDetMap::value_type(pa->geographicalId(), pa));
}

void MTTGeometry::add(MTTLayer* ly) {
	// add layers to the dets, Layers and Map
	theDets.push_back(ly);
	theLayers.push_back(ly);
	theMap.insert(MTTDetMap::value_type(ly->geographicalId(), ly));
}

void MTTGeometry::add(MTTStrip* st) {
	// add strips to the dets, Strips and Map
	theDets.push_back(st);
	theStrips.push_back(st);
	theMap.insert(MTTDetMap::value_type(st->geographicalId(), st));
}

void MTTGeometry::add(MTTTile* t) {
	// add tiles to the dets, Tiles and Map
	theDetUnits.push_back(t);
	theDets.push_back(t);
	theTiles.push_back(t);
	theMap.insert(MTTDetMap::value_type(t->geographicalId(), t));
}

const MTTGeometry::DetUnitContainer& MTTGeometry::detUnits() const {
	return theDetUnits;
}

const MTTGeometry::DetContainer& MTTGeometry::dets() const {
	return theDets;
}

const MTTGeometry::DetIdContainer& MTTGeometry::detUnitIds() const {
	// return ids of minimum unit det (tiles)
	static DetIdContainer theDetUnitIds;
	return theDetUnitIds;
}

const MTTGeometry::DetIdContainer& MTTGeometry::detIds() const {
	// return ids of all det
	static DetIdContainer theDetIds;
	return theDetIds;
}

const GeomDetUnit* MTTGeometry::idToDetUnit(DetId id) const {
	// ensure idToDet(id) points to a GeomDetUnit and cast id to det
	return dynamic_cast<const GeomDetUnit*> (idToDet(id));
}

const GeomDet* MTTGeometry::idToDet(DetId id) const {
	//return id of tiles in the map
	MTTTileId tId(id.rawId());
	MTTDetMap::const_iterator i = theMap.find(tId);
	return (i != theMap.end()) ? i->second : 0;
}

const std::vector<MTTPanel*>& MTTGeometry::panels() const {
	return thePanels;
}

const std::vector<MTTLayer*>& MTTGeometry::layers() const {
	return theLayers;
}

const std::vector<MTTStrip*>& MTTGeometry::strips() const {
	return theStrips;
}

const std::vector<MTTTile*>& MTTGeometry::tiles() const {
	return theTiles;
}

const MTTPanel* MTTGeometry::panel(MTTPanelId id) const {
	return (const MTTPanel*) (idToDet(id));
}

const MTTLayer* MTTGeometry::layer(MTTLayerId id) const {
	return (const MTTLayer*) (idToDet(id));
}

const MTTStrip* MTTGeometry::strip(MTTStripId id) const {
	return (const MTTStrip*) (idToDet(id));
}

const MTTTile* MTTGeometry::tile(MTTTileId id) const {
	return (const MTTTile*) (idToDet(id));
}
