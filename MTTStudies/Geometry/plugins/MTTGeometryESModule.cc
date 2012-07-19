/** \file
 *
 *  $Date: 2012/07/16 11:35:14 $
 *  $Revision: 1.1 $
 *  \author N. Amapane - CERN
 */

#include "MTTGeometryESModule.h"
#include <MTTStudies/Geometry/interface/MTTGeometryBuilderFromDDD.h>

#include <Geometry/Records/interface/IdealGeometryRecord.h>
#include <Geometry/Records/interface/MuonNumberingRecord.h>

// Alignments
#include "CondFormats/Alignment/interface/DetectorGlobalPosition.h"
#include "CondFormats/Alignment/interface/AlignmentErrors.h"
#include "CondFormats/AlignmentRecord/interface/GlobalPositionRcd.h"

#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/Framework/interface/ESTransientHandle.h>
#include <FWCore/Framework/interface/ModuleFactory.h>

#include <memory>
#include <iostream>

using namespace edm;
using namespace std;

MTTGeometryESModule::MTTGeometryESModule(const edm::ParameterSet & p)

{
    setWhatProduced(this, dependsOn(&MTTGeometryESModule::geometryCallback_) );
}

MTTGeometryESModule::~MTTGeometryESModule(){}

boost::shared_ptr<MTTGeometry>
MTTGeometryESModule::produce(const MuonGeometryRecord & record) {
	return _mttGeometry;
}

void MTTGeometryESModule::geometryCallback_( const MuonNumberingRecord& record ) {
  //
  // Called whenever the muon numbering (or ideal geometry) changes
  //

  _mttGeometry = boost::shared_ptr<MTTGeometry>(new MTTGeometry );
  edm::ESHandle<MuonDDDConstants> mdc;
  record.get( mdc );

  edm::ESTransientHandle<DDCompactView> cpv;
  record.getRecord<IdealGeometryRecord>().get(cpv);

  MTTGeometryBuilderFromDDD builder;
  builder.build(_mttGeometry, &(*cpv), *mdc);

}

DEFINE_FWK_EVENTSETUP_MODULE(MTTGeometryESModule);
