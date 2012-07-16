#ifndef MTTGeometry_MTTPanel_h
#define MTTGeometry_MTTPanel_h

#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "DataFormats/MuonDetId/interface/MTTPanelId.h"
#include "DataFormats/MuonDetId/interface/MTTLayerId.h"
#include "DataFormats/MuonDetId/interface/MTTStripId.h"

class MTTLayer;
class MTTStrip;

class MTTPanel: public GeomDet {

public:
	/// Constructor
	MTTPanel(MTTPanelId id, const ReferenceCountingPointer<BoundPlane>& plane);

	/// Destructor
	virtual ~MTTPanel();

	/// Return the MTTPanelId of this panel
	MTTPanelId id() const;

	// Which subdetector
	virtual SubDetector subDetector() const {
		return GeomDetEnumerators::MTT;
	}

	/// equal if the id is the same
	bool operator==(const MTTPanel& pa) const;

	/// Add Ly to the panel which takes ownership
	void add(MTTLayer* ly);

	/// Return the Layers in the Panel
	virtual std::vector<const GeomDet*> components() const;

	/// Return the sub-component (layer or strip) with a given id in this panel
	virtual const GeomDet* component(DetId id) const;

	/// Return the Layers in the Panel
	const std::vector<const MTTLayer*>& layers() const;

	/// Return the Layer corresponding to the given id
	const MTTLayer* layer(MTTLayerId id) const;

	/// Return the given layer.
	/// Layers are numbered 1 (phi), 2 (Z), 3 (phi)
	const MTTLayer* layer(int ily) const;

	/// Return the strip corresponding to the given id
	const MTTStrip* strip(MTTStripId id) const;

private:

	MTTPanelId theId;

	// The Panel owns its Ly
	std::vector<const MTTLayer*> theLayers;

};
#endif
