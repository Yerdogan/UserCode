#ifndef MTTGeometry_MTTGeometry_h
#define MTTGeometry_MTTGeometry_h

#include <DataFormats/DetId/interface/DetId.h>
#include <Geometry/CommonDetUnit/interface/TrackingGeometry.h>
#include "MTTStudies/Geometry/interface/MTTPanel.h"
#include "MTTStudies/Geometry/interface/MTTLayer.h"
#include "MTTStudies/Geometry/interface/MTTStrip.h"
#include "MTTStudies/Geometry/interface/MTTTile.h"
#include <vector>
#include <map>

class GeomDetType;
class GeomDetUnit;

class MTTGeometry: public TrackingGeometry {

	typedef std::map<DetId, GeomDet*> MTTDetMap;

public:
	/// Default constructor
	MTTGeometry();

	/// Destructor
	virtual ~MTTGeometry();

	//---- Base class' interface

	// Return a vector of all det types
	virtual const DetTypeContainer& detTypes() const;

	// Return a vector of all GeomDetUnit
	virtual const DetUnitContainer& detUnits() const;

	// Return a vector of all GeomDet (including all GeomDetUnits)
	virtual const DetContainer& dets() const;

	// Return a vector of all GeomDetUnit DetIds
	virtual const DetIdContainer& detUnitIds() const;

	// Returm a vector of all GeomDet DetIds (including those of GeomDetUnits)
	virtual const DetIdContainer& detIds() const;

	// Return the pointer to the GeomDetUnit corresponding to a given DetId
	virtual const GeomDetUnit* idToDetUnit(DetId) const;

	// Return the pointer to the GeomDet corresponding to a given DetId
	virtual const GeomDet* idToDet(DetId) const;

	//---- Extension of the interface

	/// Return a vector of all Layers
	const std::vector<MTTPanel*>& panels() const;

	/// Return a vector of all Layers
	const std::vector<MTTLayer*>& layers() const;

	/// Return a vector of all Strip
	const std::vector<MTTStrip*>& strips() const;

	/// Return a vector of all tiles
	const std::vector<MTTTile*>& tiles() const;

	/// Return a panel given its id
	const MTTPanel* panel(MTTPanelId id) const;

	/// Return a layer given its id
	const MTTLayer* layer(MTTLayerId id) const;

	/// Return a strip given its id
	const MTTStrip* strip(MTTStripId id) const;

	/// Return a tile given its id
	const MTTTile* tile(MTTTileId id) const;

private:

	friend class MTTGeometryBuilderFromDDD;
	friend class MTTGeometryBuilderFromCondDB;

	friend class GeometryAligner;

	/// Add a MTTPanel to Geometry
	void add(MTTPanel* pa);

	/// Add a MTTLayer to Geometry
	void add(MTTLayer* ly);

	/// Add a MTTStrip to Geometry
	void add(MTTStrip* st);

	/// Add a MTTTile to Geometry
	void add(MTTTile* t);

	std::vector<MTTPanel*> thePanels;
	std::vector<MTTLayer*> theLayers;
	std::vector<MTTStrip*> theStrips;
	std::vector<MTTTile*> theTiles;

	// Map for efficient lookup by DetId
	MTTDetMap theMap;

	// These are used rarely; they could be computed at runtime
	// to save memory.
	DetUnitContainer theDetUnits; // all tiles
	DetContainer theDets; // all panels, layers, strips, tiles


};

#endif
