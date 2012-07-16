import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")


process = cms.Process("Demo")

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")
from Geometry.MttCommonData.cmsMttextendedGeom_cfi import * 
process.XMLIdealGeometryESSource=XMLIdealGeometryESSource
process.load("Geometry.MTTGeometryBuilder.mttGeometry_cfi")
#FileService for histograms
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('file:/user/erdogan/output/test_xmlmerge.root')
)

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('cout','job'),
    categories = cms.untracked.vstring('FwkJob', 'HitStudy'),
    debugModules = cms.untracked.vstring('*'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING'),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(-1)
        ),
        HitStudy = cms.untracked.PSet(
            limit = cms.untracked.int32(-1),
        ),
        FwkJob = cms.untracked.PSet(
            limit = cms.untracked.int32(-1)
        )
    ),
        job = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO'),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(-1)
        ),
        HitStudy = cms.untracked.PSet(
            limit = cms.untracked.int32(-1),
        ),
        FwkJob = cms.untracked.PSet(
            limit = cms.untracked.int32(-1)
        )
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles=cms.untracked.vstring()
readFiles.extend([
    'file:/user/erdogan/workspace/mtt_development/CMSSW_5_2_0/src/SingleMuPt100_cfi_py_GEN_SIM_DIGI.root'])
process.source = cms.Source("PoolSource",fileNames=readFiles)

process.demo = cms.EDAnalyzer('SimpleSimHitAnalyzer'
)

process.p = cms.Path(process.demo)
