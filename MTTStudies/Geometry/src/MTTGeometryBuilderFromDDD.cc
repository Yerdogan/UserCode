/*
 * MTTGeometryBuilderFromDDD.cc
 *
 *  Created on: Apr 25, 2012
 *      Author: maanen
 */

#include <MTTStudies/Geometry/interface/MTTGeometryBuilderFromDDD.h>

#include <MTTStudies/Geometry/interface/MTTGeometry.h>
#include <MTTStudies/Geometry/interface/MTTPanel.h>
#include <MTTStudies/Geometry/interface/MTTLayer.h>
#include <MTTStudies/Geometry/interface/MTTStrip.h>
#include <MTTStudies/Geometry/interface/MTTTile.h>

#include <DetectorDescription/Core/interface/DDFilter.h>
#include <DetectorDescription/Core/interface/DDFilteredView.h>
#include <DetectorDescription/Core/interface/DDSolid.h>
#include "Geometry/MuonNumbering/interface/MuonDDDNumbering.h"
#include "Geometry/MuonNumbering/interface/MuonBaseNumber.h"
#include "MTTStudies/Geometry/interface/MTTNumberingScheme.h"
#include "MTTStudies/MTTDetId/interface/MTTPanelId.h"
#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include "DataFormats/GeometrySurface/interface/RectangularPlaneBounds.h"

MTTGeometryBuilderFromDDD::~MTTGeometryBuilderFromDDD() {}

MTTGeometryBuilderFromDDD::MTTGeometryBuilderFromDDD() {}

RCPPlane MTTGeometryBuilderFromDDD::plane(const DDFilteredView & fv, const Bounds & bounds) const
{
	 // extract the position
	  const DDTranslation & trans(fv.translation());

	  const Surface::PositionType posResult(float(trans.x()/cm),
	                                        float(trans.y()/cm),
	                                        float(trans.z()/cm));
	  // now the rotation
	  //  DDRotationMatrix tmp = fv.rotation();
	  // === DDD uses 'active' rotations - see CLHEP user guide ===
	  //     ORCA uses 'passive' rotation.
	  //     'active' and 'passive' rotations are inverse to each other
	  //  DDRotationMatrix tmp = fv.rotation();
	  DDRotationMatrix rotation = fv.rotation();//REMOVED .Inverse();
	  DD3Vector x, y, z;
	  rotation.GetComponents(x,y,z);
	//   std::cout << "INVERSE rotation by its own operator: "<< fv.rotation() << std::endl;
	//   std::cout << "INVERSE rotation manually: "
	// 	    << x.X() << ", " << x.Y() << ", " << x.Z() << std::endl
	// 	    << y.X() << ", " << y.Y() << ", " << y.Z() << std::endl
	// 	    << z.X() << ", " << z.Y() << ", " << z.Z() << std::endl;

	  Surface::RotationType rotResult(float(x.X()),float(x.Y()),float(x.Z()),
	                                  float(y.X()),float(y.Y()),float(y.Z()),
	                                  float(z.X()),float(z.Y()),float(z.Z()));

	//   std::cout << "rotation by its own operator: "<< tmp << std::endl;
	//   DD3Vector tx, ty,tz;
	//   tmp.GetComponents(tx, ty, tz);
	//   std::cout << "rotation manually: "
	// 	    << tx.X() << ", " << tx.Y() << ", " << tx.Z() << std::endl
	// 	    << ty.X() << ", " << ty.Y() << ", " << ty.Z() << std::endl
	// 	    << tz.X() << ", " << tz.Y() << ", " << tz.Z() << std::endl;

	  return RCPPlane( new BoundPlane( posResult, rotResult, bounds));
}

void MTTGeometryBuilderFromDDD::buildGeometry(boost::shared_ptr<MTTGeometry> theGeometry, DDFilteredView & fv, const MuonDDDConstants & muonConstants) const
{
	fv.print();
	bool doPanel = fv.firstChild();
	// Loop on chambers
	int PaCounter = 0;
	while (doPanel) {
		PaCounter++;
		//	    DDValue val("Type");
		//	    const DDsvalues_type params(fv.mergedSpecifics());
		//	    std::string type;
		//	    if (DDfetch(&params,val)) type = val.strings()[0];
		//	    // FIXME
		//	    val=DDValue("FEPos");
		//	    std::string FEPos;
		//	    if (DDfetch(&params,val)) FEPos = val.strings()[0];
		MTTPanel* panel = buildPanel(fv, muonConstants);
		fv.print();
		// Loop on Layers
		bool doL = fv.firstChild();
		int LCounter = 0;
		while (doL) {
			LCounter++;
			MTTLayer* layer = buildLayer(fv, panel, muonConstants);
			theGeometry->add(layer);
			bool doStr = fv.firstChild();
			int StrCounter = 0;
			// Loop on Strips
			while (doStr) {
				StrCounter++;
				MTTStrip* strip = buildStrip(fv, layer, muonConstants);
				theGeometry->add(strip);
				bool doTi = fv.firstChild();
				int TiCounter = 0;
				while (doTi) {
					fv.firstChild();
					TiCounter++;
					MTTTile* tile = buildTile(fv, strip, muonConstants, TiCounter);
					theGeometry->add(tile);
					fv.print();
					//fv.parent();

					fv.parent();
					doTi = fv.nextSibling();
					fv.print();
				} //Tiles
				fv.parent();
				doStr = fv.nextSibling(); // go to next Strip
			} // Strips

			fv.parent();
			doL = fv.nextSibling(); // go to next Layer
		} // Layers
		theGeometry->add(panel);

		fv.parent();
		doPanel = fv.nextSibling(); // go to next Panel
	} // Panels

}



std::vector<double> MTTGeometryBuilderFromDDD::extractParameters(DDFilteredView & fv) const
{
	 std::vector<double> par;
	  if (fv.logicalPart().solid().shape() != ddbox) {
	    DDBooleanSolid bs(fv.logicalPart().solid());
	    DDSolid A = bs.solidA();
	    while (A.shape() != ddbox) {
	      DDBooleanSolid bs(A);
	      A = bs.solidA();
	    }
	    par=A.parameters();
	  } else {
	    par = fv.logicalPart().solid().parameters();
	  }
	  return par;
}



void MTTGeometryBuilderFromDDD::build(boost::shared_ptr<MTTGeometry> theGeometry, const DDCompactView *cview, const MuonDDDConstants & muonConstants)
{

	  std::string attribute = "MTTStuff";
	  std::string value     = "MuBarrelMTT";
	  DDValue val(attribute, value, 0.0);

	  // Asking only for the Muon MTTs
	  DDSpecificsFilter filter;
	  filter.setCriteria(val,  // name & value of a variable
			     DDSpecificsFilter::matches,
			     DDSpecificsFilter::AND,
			     true, // compare strings otherwise doubles
			     true  // use merged-specifics or simple-specifics
			     );
	  DDFilteredView fview(*cview);
	  fview.addFilter(filter);
	  buildGeometry(theGeometry, fview, muonConstants);
	}



MTTPanel *MTTGeometryBuilderFromDDD::buildPanel(DDFilteredView & fv, const MuonDDDConstants & muonConstants) const
{
	  MuonDDDNumbering mdddnum (muonConstants);
	  MTTNumberingScheme mttnum (muonConstants);
	  int rawid = mttnum.getDetId(mdddnum.geoHistoryToBaseNumber(fv.geoHistory()));
	  MTTPanelId detId(rawid);

	  // Chamber specific parameter (size)
	  // FIXME: some trouble for boolean solids?
	  std::vector<double> par = extractParameters(fv);

	  float width = par[0]/cm;     //
	  float length = par[1]/cm;    //
	  float thickness = par[2]/cm; //

	  ///SL the definition of length, width, thickness depends on the local reference frame of the Det
	  // width is along local X
	  // length is along local Y
	  // thickness is long local Z
	  RectangularPlaneBounds bound(width, length, thickness);

	  RCPPlane surf(plane(fv,bound));

	  MTTPanel* panel = new MTTPanel(detId, surf);

	  return panel;
	}


MTTLayer *MTTGeometryBuilderFromDDD::buildLayer(DDFilteredView & fv, MTTPanel *panel, const MuonDDDConstants & muonConstants) const
{
	  MuonDDDNumbering mdddnum(muonConstants);
	  MTTNumberingScheme dtnum(muonConstants);
	  int rawid = dtnum.getDetId(mdddnum.geoHistoryToBaseNumber(fv.geoHistory()));
	  MTTLayerId layerId(rawid);

	  // Slayer specific parameter (size)
	  std::vector<double> par = extractParameters(fv);

	  float width = par[0]/cm;     // r-phi  dimension - changes in different chambers
	  float length = par[1]/cm;    // z      dimension - constant 126.8 cm
	  float thickness = par[2]/cm; // radial thickness - almost constant about 20 cm

	  RectangularPlaneBounds bound(width, length, thickness);

	  // Ok this is the layer position...
	  RCPPlane surf(plane(fv,bound));

	  MTTLayer* layer = new MTTLayer(layerId, surf, panel);

	  //LocalPoint lpos(10,20,30);
	  //GlobalPoint gpos=slayer->toGlobal(lpos);

	  // add to the chamber
	  panel->add(layer);
	  return layer;
}


MTTStrip *MTTGeometryBuilderFromDDD::buildStrip(DDFilteredView & fv, MTTLayer *layer, const MuonDDDConstants & muonConstants) const
{
	  MuonDDDNumbering mdddnum(muonConstants);
	  MTTNumberingScheme mttnum(muonConstants);
	  int rawid = mttnum.getDetId(mdddnum.geoHistoryToBaseNumber(fv.geoHistory()));
	  MTTStripId StrId(rawid);

	  // Slayer specific parameter (size)
	  std::vector<double> par = extractParameters(fv);

	  float width = par[0]/cm;
	  float length = par[1]/cm;
	  float thickness = par[2]/cm;

	  RectangularPlaneBounds bound(width, length, thickness);

	  // Ok this is the slayer position...
	  RCPPlane surf(plane(fv,bound));

	  MTTStrip* strip = new MTTStrip(StrId, surf, layer);

	  //LocalPoint lpos(10,20,30);
	  //GlobalPoint gpos=slayer->toGlobal(lpos);

	  // add to the chamber
	  layer->add(strip);
	  return strip;
}


MTTTile *MTTGeometryBuilderFromDDD::buildTile(DDFilteredView & fv, MTTStrip *strip, const MuonDDDConstants & muonConstants,int TiCounter) const
{
	  MuonDDDNumbering mdddnum(muonConstants);
	  MTTNumberingScheme mttnum(muonConstants);
	  uint32_t rawid = mttnum.getDetId(mdddnum.geoHistoryToBaseNumber(fv.geoHistory()));
	  MTTStripId StrId(strip->id());
	  MTTTileId TileId(strip->id(),TiCounter);
	 // std::cout << "--Wheel: "<<TileId.wheel()<<" Sector: "<<TileId.sector()<<" Layer: "<<TileId.layer()<<" Strip: "<<TileId.strip()<<" Tile: "<<TiCounter<<"--"<<std::endl;

	  // Layer specific parameter (size)
	  std::vector<double> par = extractParameters(fv);
	  float width = par[0]/cm;
	  float length = par[1]/cm;
	  float thickness = par[2]/cm;

	  // define Bounds
	  RectangularPlaneBounds bound(width, length, thickness);

	  RCPPlane surf(plane(fv,bound));

//	  // Loop on wires
//	  bool doWire = fv.firstChild();
//	  int WCounter=0;
//	  int firstWire=fv.copyno();
//	  par = extractParameters(fv);
//	  float wireLength = par[1]/cm;
//	  while (doWire) {
//	    WCounter++;
//	    doWire = fv.nextSibling(); // next wire
//	  }
//	  //int lastWire=fv.copyno();
//	  DTTopology topology(firstWire, WCounter, wireLength);
//
//	  DTLayerType layerType;

	  MTTTile* tile = new MTTTile(TileId, surf, strip);

	  strip->add(tile);
	  return tile;
}
