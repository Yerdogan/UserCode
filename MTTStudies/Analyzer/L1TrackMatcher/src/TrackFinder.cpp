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
#include "TMath.h"
TrackFinder::TrackFinder() {
}

bool TrackFinder::isTrackInTile(uint32_t tileId)
{
return true;
}

TrackFinder::TrackFinder(const MTTGeometry *mttGeometry) {
	theGeometry=mttGeometry;
	this->fillMap();
}

uint32_t TrackFinder::findCrossing(const GlobalVector& mom,const GlobalPoint& vtx){
	std::vector<MTTTile*> tiles=theGeometry->tiles();
	std::vector<MTTTile*>::iterator iTile=tiles.begin();

	for(;iTile!=tiles.end();++iTile) {
		GlobalVector nv=(*iTile)->surface().normalVector();
		float d=(*iTile)->surface().localZ(GlobalPoint(0,0,0));
		float s=(vtx.x()*nv.x()+vtx.y()*nv.y()+vtx.z()*nv.z()-d)/(nv.dot(mom));
		GlobalPoint is=vtx+s*mom;
		if((*iTile)->surface().bounds().inside((*iTile)->toLocal(is)))
			return (*iTile)->id().rawId();
	}
	return 0;
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

		GlobalPoint(0,0,0);
		etaMap[(*iTile)->id().rawId()].first=(*iTile)->surface().zSpan().first;
		etaMap[(*iTile)->id().rawId()].second=(*iTile)->surface().zSpan().second;
//		if(debug) {
			MTTTileId ID((*iTile)->id());
			std::cout << "--Wheel: "<<ID.wheel()<<" Sector: "<<ID.sector()<<" Layer: "<<ID.layer()<<" Strip: "<<ID.strip()<<" Tile: "<<ID.tile()<<"--"<<std::endl;
			std::cout<<"PhiMin= "<<phiMap[(*iTile)->id().rawId()].first<<" PhiMax= "<<phiMap[(*iTile)->id().rawId()].second<<std::endl;
			std::cout<<"zMin= "<<etaMap[(*iTile)->id().rawId()].first<<" zMax= "<<etaMap[(*iTile)->id().rawId()].second<<std::endl;
			std::cout<<"localZ= "<<(*iTile)->surface().localZ(GlobalPoint(0,0,0))<<std::endl;
			std::cout << "---------------"<<std::endl;
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
