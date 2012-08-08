import FWCore.ParameterSet.Config as cms

# Ideal geometry, needed for simulation
from Geometry.CMSCommonData.cmsIdealGeometryXML_cfi import *
XMLIdealGeometryESSource.geomXMLFiles.append('MTTStudies/Geometry/data/mttalgo.xml')
XMLIdealGeometryESSource.geomXMLFiles.append('MTTStudies/Geometry/data/mttsens.xml')
XMLIdealGeometryESSource.geomXMLFiles.append('MTTStudies/Geometry/data/mttNumbering.xml')
