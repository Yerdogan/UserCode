# Auto generated configuration file
# using: 
# Revision: 1.372 
# Source: /local/reps/CMSSW.admin/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/Generator/python/SingleMuPt100_cfi.py -s GEN,SIM,DIGI --conditions FrontierConditions_CMS.GlobalTag,DESIGN60_V3::All --eventcontent RAWSIM -n 1 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('DIGI')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
#process.load('Configuration.StandardSequences.GeometryAll_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic7TeV2011Collision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('MTTStudies.Geometry.cmsMttextendedGeom_1Layer_cfi') 
process.load('Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi')
process.load("MTTStudies.Geometry.mttGeometry_cfi")
process.load("MTTStudies.MTTDigiProducer.mttdigitizer_cfi")
process.muonDigi+=process.simMuonMTTDigis
process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")
process.g4SimHits.MTTSD=cms.PSet(EnergyThresholdForHistoryInGeV = cms.double(0.0005),EnergyThresholdForPersistencyInGeV = cms.double(0.001))
#modify tracking particles to include mtt
#process.mergedtruth.simHitCollections.muon.append("g4SimHitsMTTHits")
#dito with mixing module
process.mixSimHits.input.append(cms.InputTag("g4SimHits","MTTHits"))
process.mixSimHits.subdets.append("MTTHits")
process.mix.mixObjects.mixSH.subdets.append('MTTHits')
process.mix.mixObjects.mixSH.input.append(cms.InputTag("g4SimHits","MTTHits"))

process.RandomNumberGeneratorService.simMuonMTTDigis = cms.PSet(
    initialSeed = cms.untracked.uint32(9700)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
process.Tracer = cms.Service("Tracer")
# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('Configuration/Generator/python/SingleMuPt100_cfi.py nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    #outputCommands = cms.untracked.vstring('keep *'),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('SingleMuPt100_cfi_py_GEN_SIM_DIGI.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition
process.RAWSIMEventContent.outputCommands.append('keep *_simMuonMTTDigis_*_*')
# Other statements

process.GlobalTag.globaltag = 'MC_52_V4A::All'

process.generator = cms.EDProducer("FlatRandomPtGunProducer",
    PGunParameters = cms.PSet(
        MaxPt = cms.double(100.01),
        MinPt = cms.double(99.99),
        PartID = cms.vint32(-13),
        MaxEta = cms.double(2.5),
        MaxPhi = cms.double(3.14159265359),
        MinEta = cms.double(-2.5),
        MinPhi = cms.double(-3.14159265359)
    ),
    Verbosity = cms.untracked.int32(0),
    psethack = cms.string('single mu pt 100'),
    AddAntiParticle = cms.bool(True),
    firstRun = cms.untracked.uint32(1)
)

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.mix+process.muonDigi)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.endjob_step,process.RAWSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

