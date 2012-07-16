#include <Geometry/MTTGeometryBuilder/interface/MTTGeometryParsFromDD.h>
#include <Geometry/MTTGeometry/interface/MTTGeometry.h>   ///TODO
#include <Geometry/MTTGeometry/interface/MTTPanel.h>
#include <Geometry/MTTGeometry/interface/MTTLayer.h>
#include <Geometry/MTTGeometry/interface/MTTStrip.h>
#include <Geometry/MTTGeometry/interface/MTTTile.h>

#include <CondFormats/GeometryObjects/interface/RecoIdealGeometry.h>
#include <DetectorDescription/Core/interface/DDFilter.h>   ///TODO
#include <DetectorDescription/Core/interface/DDFilteredView.h>
#include <DetectorDescription/Core/interface/DDSolid.h>
#include <Geometry/MuonNumbering/interface/MuonDDDNumbering.h>
#include <Geometry/MuonNumbering/interface/MuonBaseNumber.h>   ///TODO
#include <Geometry/MuonNumbering/interface/MTTNumberingScheme.h>
#include <DataFormats/MuonDetId/interface/MTTPanelId.h>
#include <DataFormats/MuonDetId/interface/MTTLayerId.h>
#include <DataFormats/MuonDetId/interface/MTTStripId.h>
#include <DataFormats/MuonDetId/interface/MTTTileId.h>
#include <CLHEP/Units/GlobalSystemOfUnits.h>   ///TODO


#include <DataFormats/GeometrySurface/interface/RectangularPlaneBounds.h>

#include <string>

using namespace std;

#include <string>

using namespace std;

MTTGeometryParsFromDD::MTTGeometryParsFromDD() {}

MTTGeometryParsFromDD::~MTTGeometryParsFromDD(){}


void MTTGeometryParsFromDD::build(const DDCompactView* cview,
                                  const MuonDDDConstants& muonConstants,
                                  RecoIdealGeometry& rig) {
  //  cout << "MTTGeometryParsFromDD::build" << endl;
  //   static const string t0 = "MTTGeometryParsFromDD::build";
  //   TimeMe timer(t0,true);

	std::string attribute = "MTTStuff";
		  std::string value     = "MuBarrelMTT";  ///TODO Is this correct, where is it used and where is it defined???
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
  buildGeometry(fview, muonConstants, rig);
  //cout << "RecoIdealGeometry " << rig.size() << endl;
}


void MTTGeometryParsFromDD::buildGeometry(DDFilteredView& fv,
                                          const MuonDDDConstants& muonConstants,
                                          RecoIdealGeometry& rig) const {
  // static const string t0 = "MTTGeometryParsFromDD::buildGeometry";
  // TimeMe timer(t0,true);

  // Loop on Panels
  bool doPanel = fv.firstChild();
  int PanelCounter=0;

  while (doPanel) {
    PanelCounter++;
    DDValue val("Type");
    const DDsvalues_type params(fv.mergedSpecifics());
    string type;
    if (DDfetch(&params,val)) type = val.strings()[0];
    // FIXME
    val=DDValue("FEPos");
    string FEPos;
    if (DDfetch(&params,val)) FEPos = val.strings()[0];
    insertPanel(fv, type, muonConstants,rig);

    // Loop on Layers
    bool doLayer = fv.firstChild();
    int LayerCounter=0;

    while (doLayer){
      LayerCounter++;
      insertLayer(fv,type, muonConstants,rig);

      // Loop on Strips
      bool doStrip = fv.firstChild();
      int StripCounter=0;

      while (doStrip) {
        StripCounter++;
        insertStrip(fv, type, muonConstants, rig);

        // Loop on Tiles
        bool doTile = fv.firstChild();
        int TileCounter=0;
        while (doTile) {
          TileCounter++;
          insertTile(fv, type, muonConstants, rig);

          // fv.parent();
          doTile = fv.nextSibling(); // go to next layer
        } // tiles

        fv.parent();
        doStrip = fv.nextSibling(); // go to next layer
      } // strips

      fv.parent();
      doLayer = fv.nextSibling(); // go to next chamber
    } // layers

    fv.parent();
    doPanel = fv.nextSibling(); // go to next SL
  } // panels
}

void MTTGeometryParsFromDD::insertPanel(DDFilteredView& fv,
                                        const std::string& type,
                                        const MuonDDDConstants& muonConstants,
                                        RecoIdealGeometry& rig) const {

  MuonDDDNumbering mdddnum(muonConstants);
  MTTNumberingScheme mttnum(muonConstants);
  int rawid = mttnum.getDetId(mdddnum.geoHistoryToBaseNumber(fv.geoHistory()));
  MTTPanelId panelId(rawid);
  //cout << "inserting Panel " << panelId << endl;

  // Panel specific parameter (size)
  vector<double> par;
  par.push_back(MTTPanelTag);   ///TODO Where is this used???
  vector<double> size = extractParameters(fv);
  par.insert(par.end(), size.begin(), size.end());

  // Ok this is the panel position...
  PosRotPair posRot(plane(fv));

  rig.insert(panelId, posRot.first, posRot.second, par);
}

void MTTGeometryParsFromDD::insertLayer(DDFilteredView& fv,
                                        const string& type,
                                        const MuonDDDConstants& muonConstants,
                                        RecoIdealGeometry& rig) const {
  MuonDDDNumbering mdddnum(muonConstants);
  MTTNumberingScheme mttnum(muonConstants);
  int rawid = mttnum.getDetId(mdddnum.geoHistoryToBaseNumber(fv.geoHistory()));
  MTTLayerId layerId(rawid);
  //cout << "inserting Layer " << layerId << endl;

  // Layer specific parameter (size)
  vector<double> par;
  par.push_back(MTTLayerTag);   ///TODO Where is this used???
  vector<double> size = extractParameters(fv);
  par.insert(par.end(), size.begin(), size.end());

  /// the definition of length, width, thickness depends on the local reference frame of the Det
  // width is along local X
  // length is along local Y
  // thickness is along local Z

  PosRotPair posRot(plane(fv));

  rig.insert(layerId, posRot.first, posRot.second, par);
}

void MTTGeometryParsFromDD::insertStrip(DDFilteredView& fv,
                                        const std::string& type,
                                        const MuonDDDConstants& muonConstants,
                                        RecoIdealGeometry& rig) const {

  MuonDDDNumbering mdddnum(muonConstants);
  MTTNumberingScheme mttnum(muonConstants);
  int rawid = mttnum.getDetId(mdddnum.geoHistoryToBaseNumber(fv.geoHistory()));
  MTTStripId stripId(rawid);
  //cout << "inserting Strip " << stripId << endl;

  // Layer specific parameter (size)
  vector<double> par;
  par.push_back(MTTStripTag);   ///TODO Where is this used???
  vector<double> size = extractParameters(fv);
  par.insert(par.end(), size.begin(), size.end());

//   // Loop on wires
//   bool doWire = fv.firstChild();
//   int WCounter=0;
//   int firstWire=fv.copyno();
//   //float wireLength = par[1]/cm;
//   while (doWire) {
//     WCounter++;
//     doWire = fv.nextSibling(); // next wire
//   }
//   vector<double> sensSize= extractParameters(fv);
//   //int lastWire=fv.copyno();
//   par.push_back(firstWire);
//   par.push_back(WCounter);
//   par.push_back(sensSize[1]);
//   fv.parent();

  PosRotPair posRot(plane(fv));

  rig.insert(stripId, posRot.first, posRot.second, par);
}

void MTTGeometryParsFromDD::insertTile(DDFilteredView& fv,
                                       const string& type,
                                       const MuonDDDConstants& muonConstants,
                                       RecoIdealGeometry& rig) const {
  MuonDDDNumbering mdddnum(muonConstants);
  MTTNumberingScheme mttnum(muonConstants);
  int rawid = mttnum.getDetId(mdddnum.geoHistoryToBaseNumber(fv.geoHistory()));
  MTTTileId tileId(rawid);
  //cout << "inserting Tile " << tileId << endl;

  // Tile specific parameter (size)
  vector<double> par;
  par.push_back(MTTTileTag);   ///TODO Where is this used???
  vector<double> size = extractParameters(fv);
  par.insert(par.end(), size.begin(), size.end());

  /// the definition of length, width, thickness depends on the local reference frame of the Det
  // width is along local X
  // length is along local Y
  // thickness is along local Z

  PosRotPair posRot(plane(fv));

  rig.insert(tileId, posRot.first, posRot.second, par);
}

vector<double> 
MTTGeometryParsFromDD::extractParameters(DDFilteredView& fv) const {
  vector<double> par;
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

MTTGeometryParsFromDD::PosRotPair
MTTGeometryParsFromDD::plane(const DDFilteredView& fv) const {
  // extract the position
  const DDTranslation & trans(fv.translation());

  std::vector<double> gtran( 3 );
  gtran[0] = (float) 1.0 * (trans.x() / cm);
  gtran[1] = (float) 1.0 * (trans.y() / cm);
  gtran[2] = (float) 1.0 * (trans.z() / cm);

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

  std::vector<double> grmat( 9 );
  grmat[0] = (float) 1.0 * x.X();
  grmat[1] = (float) 1.0 * x.Y();
  grmat[2] = (float) 1.0 * x.Z();

  grmat[3] = (float) 1.0 * y.X();
  grmat[4] = (float) 1.0 * y.Y();
  grmat[5] = (float) 1.0 * y.Z();

  grmat[6] = (float) 1.0 * z.X();
  grmat[7] = (float) 1.0 * z.Y();
  grmat[8] = (float) 1.0 * z.Z();

//   std::cout << "rotation by its own operator: "<< tmp << std::endl;
//   DD3Vector tx, ty,tz;
//   tmp.GetComponents(tx, ty, tz);
//   std::cout << "rotation manually: "
// 	    << tx.X() << ", " << tx.Y() << ", " << tx.Z() << std::endl
// 	    << ty.X() << ", " << ty.Y() << ", " << ty.Z() << std::endl
// 	    << tz.X() << ", " << tz.Y() << ", " << tz.Z() << std::endl;

  return pair<std::vector<double>, std::vector<double> >(gtran, grmat);
}
