#ifndef Geometry_MTTGeometry_MTTTopology_H
#define Geometry_MTTGeometry_MTTTopology_H

#include "Geometry/CommonTopologies/interface/Topology.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"

class MTTTopology: public Topology {
public:

	MTTTopology(int, int, double);

	LocalPoint localPosition(const MeasurementPoint&) const;

	LocalError localError(const MeasurementPoint&, const MeasurementError&) const;

	MeasurementPoint measurementPosition(const LocalPoint&) const;

	MeasurementError measurementError(const LocalPoint&, const LocalError&) const;

	int channel(const LocalPoint& p) const;

	virtual ~MTTTopology() {
	}
};

#endif
