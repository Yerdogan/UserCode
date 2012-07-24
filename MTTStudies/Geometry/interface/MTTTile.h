#ifndef MTTTILE_H
#define MTTTILE_H

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "MTTStudies/Geometry/interface/MTTTopology.h"
#include "MTTStudies/Geometry/interface/MTTTileType.h"
/* Collaborating Class Declarations */
#include "MTTStudies/MTTDetId/interface/MTTTileId.h"

class MTTStrip;
class MTTLayer;
class MTTPanel;

class MTTTile: public GeomDetUnit {

public:

	MTTTile(MTTTileId id, ReferenceCountingPointer<BoundPlane>& plane, const MTTStrip* st);

	virtual ~MTTTile();

	/// Return the DetId of this tile
	MTTTileId id() const;

	const MTTStrip* strip() const ;

	const MTTLayer* layer() const;

	const MTTPanel* panel() const;

	/// True if the id are the same
	bool operator==(const MTTTile& l) const;

	virtual std::vector<const GeomDet*> components() const;

	const GeomDetType& type() const;

	const Topology& topology() const;

private:
	MTTTileId theId;
	const MTTStrip* theStrip;
	const MTTTopology theTopo;
	MTTTileType theType;
protected:

};
#endif // MTTTILE_H
