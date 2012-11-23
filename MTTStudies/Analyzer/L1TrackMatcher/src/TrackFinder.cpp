/*
 * TrackFinder.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: maanen
 */

#include "TrackFinder.h"
#include "MTTStudies/Geometry/interface/MTTGeometry.h"
#include "MTTStudies/Geometry/interface/MTTTile.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "TrackingTools/GeomPropagators/interface/StraightLinePlaneCrossing.h"
#include "TMath.h"
TrackFinder::TrackFinder() {
}


bool TrackFinder::hasTileDigi(uint32_t tileId) {
	//TODO: remove this!!
	//return true;
	for(std::vector<MTTTile*>::const_iterator iTile=TilesWithDigi->begin();iTile!=TilesWithDigi->end();++iTile)
		if((*iTile)->id().rawId()==tileId)
			return true;
	return false;
}

bool TrackFinder::isTrackInTile(uint32_t tileId) {
	return false;
}


std::vector<uint32_t> TrackFinder::getMatchedDigis(const std::vector<uint32_t>& tileIds){
	std::vector<uint32_t> res;
	uint32_t tempId=0;
	for(std::vector<uint32_t>::const_iterator iTileId=tileIds.begin();iTileId!=tileIds.end();++iTileId){
		if(hasTileDigi(tempId=*iTileId))
			res.push_back(tempId);
		if(hasTileDigi(tempId=moveRight(*iTileId)))
			res.push_back(tempId);
		if(hasTileDigi(tempId=moveRight(moveForward(*iTileId))))
			res.push_back(tempId);
		if(hasTileDigi(tempId=moveRight(moveBackward(*iTileId))))
			res.push_back(tempId);
		if(hasTileDigi(tempId=moveLeft(*iTileId)))
			res.push_back(tempId);
		if(hasTileDigi(tempId=moveLeft(moveForward(*iTileId))))
			res.push_back(tempId);
		if(hasTileDigi(tempId=moveLeft(moveBackward(*iTileId))))
			res.push_back(tempId);
		if(hasTileDigi(tempId=moveForward(*iTileId)))
			res.push_back(tempId);
		if(hasTileDigi(tempId=moveBackward(*iTileId)))
			res.push_back(tempId);
	}
	return res;
}

std::pair<uint32_t,bool> TrackFinder::isTrackInNeighbour(uint32_t tileId){

	uint32_t tempId=0;

	if(hasTileDigi(tempId=moveRight(tileId)))
		return std::pair<uint32_t,bool>(tempId,true);
	if(hasTileDigi(tempId=moveRight(moveForward(tileId))))
		return std::pair<uint32_t,bool>(tempId,true);
	if(hasTileDigi(tempId=moveRight(moveBackward(tileId))))
		return std::pair<uint32_t,bool>(tempId,true);
	if(hasTileDigi(tempId=moveLeft(tileId)))
		return std::pair<uint32_t,bool>(moveLeft(tileId),true);
	if(hasTileDigi(tempId=moveLeft(moveForward(tileId))))
		return std::pair<uint32_t,bool>(tempId,true);
	if(hasTileDigi(tempId=moveLeft(moveBackward(tileId))))
		return std::pair<uint32_t,bool>(tempId,true);
	if(hasTileDigi(tempId=moveForward(tileId)))
		return std::pair<uint32_t,bool>(tempId,true);
	if(hasTileDigi(tempId=moveBackward(tileId)))
		return std::pair<uint32_t,bool>(tempId,true);
	return std::pair<uint32_t,bool>(0,false);
}


std::pair<uint32_t,bool> TrackFinder::isTrackInLeftNeighbour(uint32_t tileId){
	MTTTileId id(tileId);
	int sector=id.sector();
	int strip=id.strip();
	int leftsector=0;
	int leftstrip=0;
	int rightsector=0;
	int rightstrip=0;

	if(strip==1){
		leftstrip=strip+1;
		leftsector=sector;
	}
	else{
		leftstrip=strip+1;
		leftsector=sector-1;
	}

	if(leftsector==-1)
		leftsector=12;

	MTTTileId leftid(id.wheel(),id.station(),leftsector,id.layer(),leftstrip,id.tile());

	if(hasTileDigi(leftid.rawId()))
		return std::pair<uint32_t,bool>(leftid.rawId(),true);
	else
		return std::pair<uint32_t,bool>(leftid.rawId(),false);
}

std::pair<uint32_t,bool> TrackFinder::isTrackInRightNeighbour(uint32_t tileId){
	MTTTileId id(tileId);
	int sector=id.sector();
	int strip=id.strip();
	int rightsector=0;
	int rightstrip=0;

	if(strip==1){
		rightstrip=strip+1;
		rightsector=sector+1;
	}
	else{
			rightstrip=strip-1;
		rightsector=sector;
	}

	if(rightsector==13)
		rightsector=1;
	MTTTileId rightid(id.wheel(),id.station(),rightsector,id.layer(),rightstrip,id.tile());
	if(hasTileDigi(rightid.rawId()))
		return std::pair<uint32_t,bool>(rightid.rawId(),true);
	else
		return std::pair<uint32_t,bool>(rightid.rawId(),false);
}

uint32_t TrackFinder::moveRight(uint32_t tileid){
	MTTTileId id(tileid);
	int sector=id.sector();
	int strip=id.strip();
	int rightsector=0;
	int rightstrip=0;

	if(strip==1){
		rightstrip=strip+1;
		rightsector=sector+1;
	}
	else{
			rightstrip=strip-1;
		rightsector=sector;
	}

	if(rightsector==13)
		rightsector=1;
	MTTTileId rightid(id.wheel(),id.station(),rightsector,id.layer(),rightstrip,id.tile());
	return rightid.rawId();
}

uint32_t TrackFinder::moveLeft(uint32_t tileid){
	MTTTileId id(tileid);
	int sector=id.sector();
	int strip=id.strip();
	int leftsector=0;
	int leftstrip=0;

	if(strip==1){
		leftstrip=strip+1;
		leftsector=sector;
	}
	else{
		leftstrip=strip+1;
		leftsector=sector-1;
	}

	if(leftsector==-1)
		leftsector=12;

	MTTTileId leftid(id.wheel(),id.station(),leftsector,id.layer(),leftstrip,id.tile());
	return leftid.rawId();
}

uint32_t TrackFinder::moveForward(uint32_t tileid){
	MTTTileId id(tileid);
	int tile=id.tile();
	int wheel=id.wheel();
	if(tile==10&&wheel<2){
		wheel++;
		tile=1;
	}
	else if(tile<10)
		tile++;

		MTTTileId fwid(wheel,id.station(),id.sector(),id.layer(),id.strip(),tile);
		return fwid.rawId();
}

uint32_t TrackFinder::moveBackward(uint32_t tileid){
	MTTTileId id(tileid);
	int tile=id.tile();
	int wheel=id.wheel();
	if(tile==1&&wheel>-2){
		wheel--;
		tile=10;
	}
	else if(tile>1)
		tile--;

		MTTTileId bwid(wheel,id.station(),id.sector(),id.layer(),id.strip(),tile);
		return bwid.rawId();;
}


TrackFinder::TrackFinder(const MTTGeometry *mttGeometry) {
	theGeometry=mttGeometry;
	this->fillMap();
}

std::vector<uint32_t> TrackFinder::findCrossing(const GlobalVector& mom,const GlobalPoint& vtx){


	std::vector<MTTTile*> tiles=theGeometry->tiles();
	std::vector<MTTTile*>::iterator iTile=tiles.begin();
	StraightLinePlaneCrossing::PositionType pos(vtx);
	StraightLinePlaneCrossing::DirectionType dir(mom);
	StraightLinePlaneCrossing cross(pos,dir);
	std::vector<uint32_t> res;
	for(;iTile!=tiles.end();++iTile) {
		if(cross.position((*iTile)->surface()).first){
			LocalPoint lp((*iTile)->toLocal(GlobalPoint(cross.position((*iTile)->surface()).second.x(),cross.position((*iTile)->surface()).second.y(),cross.position((*iTile)->surface()).second.z())));
			LocalPoint lp2(lp.x(),lp.y(),0);
			if(((*iTile)->surface().bounds().inside(lp))){
				//   std::cout<<" global transverse "<<cross.position((*iTile)->surface()).second.transverse()<<std::endl;
				//	return (*iTile)->id().rawId();
				res.push_back((*iTile)->id().rawId());
			}
		}
	}
	return res;
}


void TrackFinder::fillMap()
{

	if (!theGeometry) {
		throw cms::Exception("Configuration")
		<< "TrackFinder requires the MTTGeometry \n which is not present in the configuration file.  You must add the service\n in the configuration file or remove the modules that require it.";
	}

	std::vector<MTTTile*> tiles=theGeometry->tiles();
	std::vector<MTTTile*>::iterator iTile=tiles.begin();
	for(;iTile!=tiles.end();++iTile) {

		if((*iTile)->id().sector()!=12){
			phiMap[(*iTile)->id().rawId()].first=(*iTile)->surface().phiSpan().first;
			if((*iTile)->surface().phiSpan().second>(*(iTile+1))->surface().phiSpan().first)
				phiMap[(*iTile)->id().rawId()].second=(*(iTile+1))->surface().phiSpan().first;
			else
				phiMap[(*iTile)->id().rawId()].second=(*(iTile+1))->surface().phiSpan().second;

			phiSectorMap[(*iTile)->id().sector()].first=(*iTile)->surface().phiSpan().first;
			phiSectorMap[(*iTile)->id().sector()].second=(*(iTile+1))->surface().phiSpan().first;
		}
		else {
			phiMap[(*iTile)->id().rawId()].first=(*iTile)->surface().phiSpan().first;
			phiMap[(*iTile)->id().rawId()].second=(*(iTile-11))->surface().phiSpan().first;

			phiSectorMap[(*iTile)->id().sector()].first=(*iTile)->surface().phiSpan().first;
			phiSectorMap[(*iTile)->id().sector()].second=(*(iTile-11))->surface().phiSpan().first;
		}

		etaMap[(*iTile)->id().rawId()].first=(*iTile)->surface().zSpan().first;
		etaMap[(*iTile)->id().rawId()].second=(*iTile)->surface().zSpan().second;
		//		if(debug) {
		MTTTileId ID((*iTile)->id());
		LogDebug("TrackFinder::fillMap") << ID <<std::endl;
		LogDebug("TrackFinder::fillMap")<<"PhiMin= "<<phiMap[(*iTile)->id().rawId()].first<<" PhiMax= "<<phiMap[(*iTile)->id().rawId()].second<<std::endl;
		LogDebug("TrackFinder::fillMap")<<"zMin= "<<etaMap[(*iTile)->id().rawId()].first<<" zMax= "<<etaMap[(*iTile)->id().rawId()].second<<std::endl;
		LogDebug("TrackFinder::fillMap")<<"localZ= "<<(*iTile)->surface().localZ(GlobalPoint(0,0,0))<<std::endl;
		LogDebug("TrackFinder::fillMap") << "---------------"<<std::endl;
		//		}
	}
}

void TrackFinder::setGeometry(const MTTGeometry* mttGeometry) {
	theGeometry=mttGeometry;
}

void TrackFinder::setTiles(const std::vector<MTTTile*>* tiles) {
	TilesWithDigi=tiles;
}


int findPhiSegment(float phi) {

	//	if(phi>0){
	//		for(int i=1;i<7;i++) {
	//			if(phi<phiSectorMap[(*iTile)->id().sector()].second&&phi>phiSectorMap[(*iTile)->id().sector()].first)
	//				return i;
	//		}
	//	//	if(phi>phiSectorMap[/*TODO*/]&&phi<TMath::Pi())
	//	}
	//	else{
	//	}
	//		for(int i=12;i>7;i++) {
	//				if(abs(phi)<abs(phiSectorMap[(*iTile)->id().sector()].first)&&abs(phi)<(phiSectorMap[(*iTile)->id().sector()].second))
	//					return i;
	//	}
	//	return segment;
	return 0;
}




TrackFinder::~TrackFinder() {
	// TODO Auto-generated destructor stub
}
