#ifndef MTTSTRIP_H
#define MTTSTRIP_H

/* Base Class Headers */
#include "Geometry/CommonDetUnit/interface/GeomDet.h"

/* Collaborating Class Declarations */
#include "DataFormats/MuonDetId/interface/MTTStripId.h"
#include "DataFormats/MuonDetId/interface/MTTTileId.h"

class MTTTile;
class MTTLayer;
class MTTPanel;

class MTTStrip: public GeomDet {

public:

	/* Constructor */
	MTTStrip(MTTStripId id, ReferenceCountingPointer<BoundPlane>& plane, const MTTLayer* ch = 0);

	/* Destructor */
	virtual ~MTTStrip();

	/* Operations */
	/// Return the DetId of this strip
	MTTStripId id() const;

	// Which subdetector
	virtual SubDetector subDetector() const {
		return GeomDetEnumerators::MTT;
	}

	/// True if id are the same
	bool operator==(const MTTStrip& st) const ;

	virtual std::vector<const GeomDet*> components() const;

	virtual const GeomDet* component(DetId id) const;

	const std::vector<const MTTTile*>& tiles() const;

	void add(MTTTile* l);

	const MTTPanel* panel() const;

	const MTTLayer* layer() const;

	const MTTTile* tile(MTTTileId id) const;

	const MTTTile* tile(int itile) const;

private:
	MTTStripId theId;

	std::vector<const MTTTile*> theTiles;
	const MTTLayer* theLayer;

protected:

};
#endif // MTTStrip_H
