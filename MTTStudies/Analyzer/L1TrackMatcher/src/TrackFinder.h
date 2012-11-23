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
class MTTTile;
//class cmsUpgrades::L1TkTrack;

class TrackFinder {
	std::map<uint32_t,std::pair<float,float> > etaMap;
	std::map<uint32_t,std::pair<float,float> > phiMap;

	std::map<int, std::pair<float,float> > phiSectorMap;
	const MTTGeometry* theGeometry;
	const std::vector<MTTTile*>* TilesWithDigi;

	std::pair<uint32_t,bool> isTrackInNeighbour(uint32_t tileId);
	std::pair<uint32_t,bool> isTrackInLeftNeighbour(uint32_t tileId);
	std::pair<uint32_t,bool> isTrackInRightNeighbour(uint32_t tileId);
	//
	int getPhiSegment(float phi);

	int getEtaSegment(float eta);

	// the following functions move a tileid to a tile left(-phi), right(+phi), forward(+z) or backward(-z)
	uint32_t moveRight(uint32_t);
	uint32_t moveLeft(uint32_t);
	uint32_t moveForward(uint32_t);
	uint32_t moveBackward(uint32_t);


public:
	TrackFinder();
	TrackFinder(const MTTGeometry* mttGeometry);
	bool isTrackInTile(uint32_t tileId);
	bool hasTileDigi(uint32_t tileId);
	virtual ~TrackFinder();
	void fillMap();
	void setGeometry(const MTTGeometry* mttGeometry);
	void setTiles(const std::vector<MTTTile*>* tiles);

	std::vector<uint32_t> findCrossing(const GlobalVector& mom,const GlobalPoint& vtx);
	std::vector<uint32_t> getMatchedDigis(const std::vector<uint32_t>&);
};

#endif /* TRACKFINDER_H_ */
