import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load('MTTStudies.Geometry.cmsMttextendedGeom_1Layer_cfi')
process.load("MTTStudies.Geometry.mttGeometry_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("EmptySource",
    # replace 'myfile.root' with the source file you want to use
    #fileNames = cms.untracked.vstring(
     #   'file:myfile.root'
  #  )
)

process.demo = cms.EDAnalyzer('L1TrackMatcher'
)


process.p = cms.Path(process.demo)
