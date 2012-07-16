#ifndef DTGeometryBuilder_DTGeometryESModule_h
#define DTGeometryBuilder_DTGeometryESModule_h

/** \class MTTGeometryESModule
 *
 *  ESProducer for MTTGeometry in MuonGeometryRecord
 */

#include <FWCore/Framework/interface/ESProducer.h>
#include "FWCore/Framework/interface/EventSetupRecordIntervalFinder.h"
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <Geometry/Records/interface/MuonGeometryRecord.h>
#include <Geometry/MTTGeometry/interface/MTTGeometry.h>

#include <string>

class MTTGeometryESModule : public edm::ESProducer {
public:
  /// Constructor
  MTTGeometryESModule(const edm::ParameterSet & p);

  /// Destructor
  virtual ~MTTGeometryESModule();

  /// Produce DTGeometry.
  boost::shared_ptr<MTTGeometry> produce(const MuonGeometryRecord& record);

private:
  void geometryCallback_( const MuonNumberingRecord& record ) ;
  boost::shared_ptr<MTTGeometry> _mttGeometry;

};
#endif



