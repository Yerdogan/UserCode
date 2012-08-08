#!/bin/bash
echo "Applying patches......\n"
patch $CMSSW_BASE/src/SimG4Core/Application/plugins/OscarProducer.cc $CMSSW_BASE/src/MTTStudies/Patches/OscarProducer_cc.patch
#patch $CMSSW_BASE/src/DetectorDescription/Algorithm/plugins/BuildFile.xml $CMSSW_BASE/src/MTTStudies/Patches/DetectorDescription_Algorithm_plugins_BuildFile.patch
patch $CMSSW_BASE/src/Geometry/CommonDetUnit/src/GeomDetEnumerators.cc $CMSSW_BASE/src/MTTStudies/Patches/GeomDetEnumerators_cc.patch
patch $CMSSW_BASE/src/Geometry/CommonDetUnit/interface/GeomDetEnumerators.h $CMSSW_BASE/src/MTTStudies/Patches/GeomDetEnumerators_h.patch
patch $CMSSW_BASE/src/Geometry/CommonDetUnit/src/GeomDetType.cc $CMSSW_BASE/src/MTTStudies/Patches/GeomDetType_cc.patch
patch $CMSSW_BASE/src/Geometry/CommonDetUnit/interface/GeomDetType.h $CMSSW_BASE/src/MTTStudies/Patches/GeomDetType_h.patch
patch $CMSSW_BASE/src/Geometry/MuonNumbering/interface/MuonSubDetector.h $CMSSW_BASE/src/MTTStudies/Patches/MuonSubDetector_h.patch
patch $CMSSW_BASE/src/Geometry/MuonNumbering/src/MuonSubDetector.cc $CMSSW_BASE/src/MTTStudies/Patches/MuonSubDetector_cc.patch
patch $CMSSW_BASE/src/SimG4CMS/Muon/BuildFile.xml $CMSSW_BASE/src/MTTStudies/Patches/simg4cms_muon_BuildFile.patch
#patch $CMSSW_BASE/src/SimG4CMS/Muon/src/module.cc $CMSSW_BASE/src/MTTStudies/Patches/simg4cms_muon_module_cc.patch



