# Auto generated configuration file
# using: 
# Revision: 1.372 
# Source: /local/reps/CMSSW.admin/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/Generator/python/SingleMuPt100_cfi.py -s GEN,SIM --conditions auto:mc --eventcontent RAWSIM -n 10 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('SIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
#process.load("SimGeneral.TrackingAnalysis.trackingParticles_cfi")
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic7TeV2011Collision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Geometry.MttCommonData.cmsMttextendedGeom_cfi import * 
process.XMLIdealGeometryESSource=XMLIdealGeometryESSource
process.maxEvents = cms.untracked.PSet(
<<<<<<< SingleMuPt100_cfi_py_GEN_SIM.py
    input = cms.untracked.int32(1)
=======
    input = cms.untracked.int32(10000)
>>>>>>> 1.7
)
process.g4SimHits.MTTSD=cms.PSet(EnergyThresholdForHistoryInGeV = cms.double(0.0005),EnergyThresholdForPersistencyInGeV = cms.double(0.001))

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.9 $'),
    annotation = cms.untracked.string('Configuration/Generator/python/SingleMuPt100_cfi.py nevts:10'),
    name = cms.untracked.string('PyReleaseValidation')
)



#process.load("SimGeneral.TrackingAnalysis.trackingParticles_cfi")
#process.mix.playback=cms.untracked.bool(False)

#modify tracking particles to include mtt
#process.mergedtruth.simHitCollections.muon.append("g4SimHitsMTTHits")
#dito with mixing module
#process.mixSimHits.input.append(cms.InputTag("g4SimHits","MTTHits"))
#process.mixSimHits.subdets.append("MTTHits")
#process.mix.mixObjects.mixSH.subdets.append('MTTHits')
#process.mix.mixObjects.mixSH.input.append(cms.InputTag("g4SimHits","MTTHits"))



# Output definition
#process.RAWSIMEventContent.outputCommands.append('keep *_mergedtruth_MergedTrackTruth_*')
process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('SingleMuPt100_cfi_py_GEN_SIM_TP.root'),
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
        MaxPt = cms.double(100.01),
        MinPt = cms.double(99.99),
        PartID = cms.vint32(-13),
        MaxEta = cms.double(1.2),
        MaxPhi = cms.double(3.14159265359),
        MinEta = cms.double(-1.2),
        MinPhi = cms.double(-3.14159265359)
    ),
    Verbosity = cms.untracked.int32(0),
    psethack = cms.string('single mu pt 100'),
    AddAntiParticle = cms.bool(False),
    firstRun = cms.untracked.uint32(1)
)


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)#+process.mix+process.trackingParticles)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

