#ifndef MTTGeometryBuilder_MTTGeometryParsFromDD_h
#define MTTGeometryBuilder_MTTGeometryParsFromDD_h

#include "DataFormats/GeometrySurface/interface/BoundPlane.h"
#include <vector>
#include <boost/shared_ptr.hpp>

class MTTGeometry;   ///TODO
class DDCompactView;
class DDFilteredView;
class MTTPanel;   ///TODO
class MTTLayer;   ///TODO
class MTTStrip;   ///TODO
class MTTTile;   ///TODO
class Bounds;   ///TODO
class MuonDDDConstants;
class RecoIdealGeometry;

class MTTGeometryParsFromDD {
  public:
    /// Constructor
    MTTGeometryParsFromDD();

    /// Destructor
    virtual ~MTTGeometryParsFromDD();

    // Operations
    void build(const DDCompactView* cview,
               const MuonDDDConstants& muonConstants,
               RecoIdealGeometry& rig) ;

    enum MTTDetTag { MTTPanelTag, MTTLayerTag, MTTStripTag, MTTTileTag };
  private:
    /// create the panel
    void insertPanel(DDFilteredView& fv,
                     const std::string& type,
                     const MuonDDDConstants& muonConstants,
                     RecoIdealGeometry& rig) const;

    /// create the layer
    void insertLayer(DDFilteredView& fv,
                     const std::string& type,
                     const MuonDDDConstants& muonConstants,
                     RecoIdealGeometry& rig) const;

    /// create the strip
    void insertStrip(DDFilteredView& fv,
                     const std::string& type,
                     const MuonDDDConstants& muonConstants,
                     RecoIdealGeometry& rig) const;

    /// create the tile
    void insertTile(DDFilteredView& fv,
                    const std::string& type,
                    const MuonDDDConstants& muonConstants,
                    RecoIdealGeometry& rig) const;

    /// get parameter also for boolean solid.
    std::vector<double> extractParameters(DDFilteredView& fv) const ;

    typedef std::pair<std::vector<double>, std::vector<double> > PosRotPair;

    PosRotPair plane(const DDFilteredView& fv) const ;

    void buildGeometry(DDFilteredView& fv,
                       const MuonDDDConstants& muonConstants,
                       RecoIdealGeometry& rig) const;

};
#endif