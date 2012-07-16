#include "Geometry/MTTGeometry/interface/MTTPanel.h"

/* Collaborating Class Header */
#include "Geometry/MTTGeometry/interface/MTTLayer.h"
#include "Geometry/MTTGeometry/interface/MTTStrip.h"

/* C++ Headers */
#include <iostream>

/* ====================================================================== */

/* Constructor */
MTTPanel::MTTPanel(MTTPanelId id, const ReferenceCountingPointer<BoundPlane>& plane) :
	GeomDet(plane), theId(id) {
	setDetId(id);
}

/* Destructor */
MTTPanel::~MTTPanel() {
	for (std::vector<const MTTLayer*>::const_iterator ily = theLayers.begin(); ily != theLayers.end(); ++ily)
		delete (*ily);
}

/* Operations */

MTTPanelId MTTPanel::id() const {
	return theId;
}

bool MTTPanel::operator==(const MTTPanel& pa) const {
	return id() == pa.id();
}

void MTTPanel::add(MTTLayer* ly) {
	theLayers.push_back(ly);
}

std::vector<const GeomDet*> MTTPanel::components() const {
	return std::vector<const GeomDet*>(theLayers.begin(), theLayers.end());
}

const GeomDet* MTTPanel::component(DetId id) const {
	// return panel components with a given id, distinguish between layer or strip
	MTTStripId sId(id.rawId());
	if (sId.strip() == 0) { // is a layer id
		return layer(sId);
	} else { // is a strip id
		return strip(sId);
	}
}

const std::vector<const MTTLayer*>& MTTPanel::layers() const {
	return theLayers;
}

const MTTLayer* MTTPanel::layer(MTTLayerId id) const {
	if (id.panelId() != theId)
		return 0; // not in this Ly!
	return layer(id.layer());
}

const MTTLayer* MTTPanel::layer(int ily) const {
	for (std::vector<const MTTLayer*>::const_iterator i = theLayers.begin(); i != theLayers.end(); ++i) {
		if ((*i)->id().layer() == ily)
			return (*i);
	}
	return 0;
}

const MTTStrip* MTTPanel::strip(MTTStripId id) const {
	return (layer(id.layer()))->strip(id.strip());
}

