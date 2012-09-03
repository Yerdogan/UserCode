/*
 * TrackFinder.h
 *
 *  Created on: Aug 29, 2012
 *      Author: maanen
 */

#ifndef TRACKFINDER_H_
#define TRACKFINDER_H_

#include <map>
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
class MTTGeometry;
//class cmsUpgrades::L1TkTrack;

class TrackFinder {
	std::map<uint32_t,std::pair<float,float> > etaMap;
	std::map<uint32_t,std::pair<float,float> > phiMap;

	std::map<int, std::pair<float,float> > phiSectorMap;
//	std::map<uint32_t, std::vector<cmsUpgrades::L1TkTrack*> > trackMap;
	const MTTGeometry* theGeometry;
	const std::vector<MTTTiles*>* TilesWithDigi;

	//
	int getPhiSegment(float phi);

	int getEtaSegment(float eta);



public:
	TrackFinder();
	TrackFinder(const MTTGeometry* mttGeometry);
	bool isTrackInTile(uint32_t tileId);
	virtual ~TrackFinder();
	void fillMap();
	void setGeometry(const MTTGeometry* mttGeometry);
	void setTiles(const std::vector<MTTTiles*>* TilesWithDigi);

	uint32_t findCrossing(const GlobalVector& mom,const GlobalPoint& vtx);
};

#endif /* TRACKFINDER_H_ */
