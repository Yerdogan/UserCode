import FWCore.ParameterSet.Config as cms

# Ideal geometry, needed for simulation
from Geometry.CMSCommonData.cmsIdealGeometryXML_cfi import *
XMLIdealGeometryESSource.geomXMLfiles.append('MTTStudies/Geometry/data/mttalgo_1Layer.xml')
XMLIdealGeometryESSource.geomXMLfiles.append('MTTStudies/Geometry/data/mttsens_1Layer.xml')
XMLIdealGeometryESSource.geomXMLfiles.append('MTTStudies/Geometry/data/mttNumbering.xml')
