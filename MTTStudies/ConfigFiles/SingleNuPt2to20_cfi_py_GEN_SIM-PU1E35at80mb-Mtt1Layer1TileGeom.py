# Auto generated configuration file
# using: 
# Revision: 1.372 
# Source: /local/reps/CMSSW.admin/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/Generator/python/SingleMuPt100_cfi.py -s GEN,SIM --conditions auto:mc --eventcontent RAWSIM -n 10 --no_exec
#dummy comment please ignore
import FWCore.ParameterSet.Config as cms

process = cms.Process('SIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Geometry.MttCommonData.mixHighLumPU_Phase2_Mtt1Layer1TileGeom_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedGauss_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Analyzer.CrossingFrameAnalyzer.crossingframeanalyzer_cfi')
from Geometry.MttCommonData.cmsMttextendedGeom_1Layer_cfi import *
process.XMLIdealGeometryESSource=XMLIdealGeometryESSource
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(3000)
)
process.g4SimHits.MTTSD=cms.PSet(EnergyThresholdForHistoryInGeV = cms.double(0.01),EnergyThresholdForPersistencyInGeV = cms.double(0.03))
#modify tracking particles to include mtt
#process.mergedtruth.simHitCollections.muon.append("g4SimHitsMTTHits")
#dito with mixing module


#dito with mixing module
process.mixSimHits.input.append(cms.InputTag("g4SimHits","MTTHits"))
process.mixSimHits.subdets.append("MTTHits")
process.mix.mixObjects.mixSH.subdets.append('MTTHits')
process.mix.mixObjects.mixSH.input.append(cms.InputTag("g4SimHits","MTTHits"))

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('Single neutrino gun with flat pt distribution and pileup'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    fileName = cms.untracked.string('SingleNuPt2to20_cfi_py_GEN_SIM-PU1E35at80mb-Mtt1Layer1TileGeom.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'MC_52_V4A::All'

process.generator = cms.EDProducer("FlatRandomPtGunProducer",
    PGunParameters = cms.PSet(
        MaxPt = cms.double(20.),
        MinPt = cms.double(2.),
        PartID = cms.vint32(13),
        MaxEta = cms.double(1.2),
        MaxPhi = cms.double(3.14159265359),
        MinEta = cms.double(-1.2),
        MinPhi = cms.double(-3.14159265359)
    ),
    Verbosity = cms.untracked.int32(0),
    psethack = cms.string('single neutrino 2<Pt<20 -3<Eta<3'),
    AddAntiParticle = cms.bool(False),
    firstRun = cms.untracked.uint32(1)
)

process.TFileService = cms.Service("TFileService", 
      fileName = cms.string("histo.root"),
      closeFileFast = cms.untracked.bool(True)
  )


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim+process.mix+process.cfAnalyzer)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath()
#process.RAWSIMoutput
# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

