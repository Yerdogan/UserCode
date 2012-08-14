import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load('MTTStudies.Geometry.cmsMttextendedGeom_1Layer_cfi')
process.load("MTTStudies.Geometry.mttGeometry_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.TFileService = cms.Service("TFileService", 
      fileName = cms.string("histo.root"),
      closeFileFast = cms.untracked.bool(True)
  )
process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/user/maanen/MttStudies/digimap/CMSSW_5_2_5/src/SingleMuPt100_cfi_py_GEN_SIM_DIGI.root'
    )
)

process.demo = cms.EDAnalyzer('DigiMap'
)


process.p = cms.Path(process.demo)

