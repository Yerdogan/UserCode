#ifndef MTTGeometry_MTTLayer_h
#define MTTGeometry_MTTLayer_h

#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "MTTStudies/Geometry/interface/MTTLayerId.h"
#include "MTTStudies/Geometry/interface/MTTStripId.h"
#include "MTTStudies/Geometry/interface/MTTTileId.h"

class MTTStrip;
class MTTTile;
class MTTPanel;
class MTTLayer: public GeomDet {

public:
	/// Constructor
	MTTLayer(MTTLayerId id, const ReferenceCountingPointer<BoundPlane>& plane, MTTPanel* pa);

	/// Destructor
	virtual ~MTTLayer();

	/// Return the MTTLayerId of this layer
	MTTLayerId id() const;

	// Which subdetector
	virtual SubDetector subDetector() const {
		return GeomDetEnumerators::MTT;
	}

	/// equal if the id is the same
	bool operator==(const MTTLayer& ly) const;

	/// Add St to the layer which takes ownership
	void add(MTTStrip* st);

	/// Return the Panel

	const MTTPanel* panel() const;

	/// Return the strips in the layer
	virtual std::vector<const GeomDet*> components() const;

	/// Return the sub-component (strip or tile) with a given id in this layer
	virtual const GeomDet* component(DetId id) const;

	/// Return the strips in the layer
	const std::vector<const MTTStrip*>& strips() const;

	/// Return the strip corresponding to the given id
	const MTTStrip* strip(MTTStripId id) const;

	/// Return the given strip.
	/// Strips are numbered 1 (phi), 2 (Z), 3 (phi)
	const MTTStrip* strip(int ist) const;

	/// Return the tile corresponding to the given id
	const MTTTile* tile(MTTTileId id) const;

private:

	MTTLayerId theId;

	// The layer owns its strip
	std::vector<const MTTStrip*> theStrips;
	const MTTPanel* thePanel;

};
#endif
