#include "Geometry/MTTGeometry/interface/MTTTopology.h"
#include <FWCore/Utilities/interface/Exception.h>

#include <iostream>

MTTTopology::MTTTopology(int a, int b, double c) {

}

LocalPoint MTTTopology::localPosition(const MeasurementPoint& mp) const {
	//FIXME
	return LocalPoint(mp.x(), mp.y());
}

LocalError MTTTopology::localError(const MeasurementPoint& /*mp*/, const MeasurementError& me) const {
	//FIXME
	return LocalError(me.uu(), 0, me.vv());
}

MeasurementPoint MTTTopology::measurementPosition(const LocalPoint& lp) const {
	//FIXME
	return MeasurementPoint(lp.x(), lp.y());
}

MeasurementError MTTTopology::measurementError(const LocalPoint& lp, const LocalError& le) const {
	//FIXME
	return measurementError(lp, le);
}

int MTTTopology::channel(const LocalPoint& lp) const {
	//FIXME
	return channel(lp);
}
