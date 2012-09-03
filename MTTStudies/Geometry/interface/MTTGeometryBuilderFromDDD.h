/*
 * MTTGeometryBuilderFromDDD.h
 *
 *  Created on: Apr 25, 2012
 *      Author: maanen
 */

#ifndef MTTGEOMETRYBUILDERFROMDDD_H_
#define MTTGEOMETRYBUILDERFROMDDD_H_

#include "DataFormats/GeometrySurface/interface/BoundPlane.h"
#include <vector>
#include <boost/shared_ptr.hpp>

class MTTGeometry;
class DDCompactView;
class DDFilteredView;
class MTTPanel;
class MTTLayer;
class MTTStrip;
class MTTTile;
class Bounds;
class MuonDDDConstants;

typedef ReferenceCountingPointer<BoundPlane> RCPPlane;

class MTTGeometryBuilderFromDDD {
public:
	virtual ~MTTGeometryBuilderFromDDD();
	MTTGeometryBuilderFromDDD();
	  void build(boost::shared_ptr<MTTGeometry> theGeometry,
	               const DDCompactView* cview,
	               const MuonDDDConstants& muonConstants);
private:
	    /// create the panel
	    MTTPanel* buildPanel(DDFilteredView& fv,
	                            const MuonDDDConstants& muonConstants) const;

	    /// create the layer
	    MTTLayer* buildLayer(DDFilteredView& fv,
	                                  MTTPanel* Panel,
	                                  const MuonDDDConstants& muonConstants) const;

	    /// create the strip
	    MTTStrip* buildStrip(DDFilteredView& fv,
	                        MTTLayer* layer,
	                        const MuonDDDConstants& muonConstants) const;
	    ///create the tile
	    MTTTile* buildTile(DDFilteredView& fv,
	                        MTTStrip* strip,
	                        const MuonDDDConstants& muonConstants,int TiCounter) const;

	    /// get parameter also for boolean solid.
	    std::vector<double> extractParameters(DDFilteredView& fv) const ;


	    RCPPlane plane(const DDFilteredView& fv,
	                   const Bounds& bounds) const ;

	    void buildGeometry(boost::shared_ptr<MTTGeometry> theGeometry,
	                       DDFilteredView& fv,
	                       const MuonDDDConstants& muonConstants) const;

};

#endif /* MTTGEOMETRYBUILDERFROMDDD_H_ */
