import FWCore.ParameterSet.Config as cms

# Ideal geometry, needed for simulation
from Configuration.StandardSequences.GeometryIdeal_cff import *
XMLIdealGeometryESSource.geomXMLFiles.append('MTTStudies/Geometry/data/mttalgo_1Layer.xml')
XMLIdealGeometryESSource.geomXMLFiles.append('MTTStudies/Geometry/data/mttsens_1Layer.xml')
XMLIdealGeometryESSource.geomXMLFiles.append('MTTStudies/Geometry/data/mttNumbering.xml')
