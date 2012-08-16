# Auto generated configuration file
# using: 
# Revision: 1.372.2.3 
# Source: /local/reps/CMSSW.admin/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: SingleNuE10_cfi.py -s GEN,SIM,DIGI --conditions=auto:mc -n 100000 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('DIGI')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('MTTStudies.Geometry.mixPU100_Phase2_Mtt1Layer1TileGeom_cff')
process.load('MTTStudies.Geometry.cmsMttextendedGeom_1Layer_cfi')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic8TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load('Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi')
process.load("MTTStudies.Geometry.mttGeometry_cfi")
process.load("MTTStudies.MTTDigiProducer.mttdigitizer_cfi")
process.muonDigi+=process.simMuonMTTDigis
process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")

#dito mixing module
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
process.g4SimHits.MTTSD=cms.PSet(EnergyThresholdForHistoryInGeV = cms.double(0.001),EnergyThresholdForPersistencyInGeV = cms.double(0.01))

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.372.2.3 $'),
    annotation = cms.untracked.string('SingleNuE10_cfi.py nevts:100'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    fileName = cms.untracked.string('ZB_GEN_SIM_DIGI-PU100.root'),
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
process.GlobalTag.globaltag = 'DESIGN52_V9::All'

process.generator = cms.EDProducer("FlatRandomEGunProducer",
    PGunParameters = cms.PSet(
        PartID = cms.vint32(12),
        MaxEta = cms.double(2.5),
        MaxPhi = cms.double(3.14159265359),
        MinEta = cms.double(-2.5),
        MinE = cms.double(9.99),
        MinPhi = cms.double(-3.14159265359),
        MaxE = cms.double(10.01)
    ),
    Verbosity = cms.untracked.int32(0),
    psethack = cms.string('single Nu E 10'),
    AddAntiParticle = cms.bool(False),
    firstRun = cms.untracked.uint32(1)
)

#remove non-needed digi
process.pdigi.remove(process.simSiStripDigis)
process.pdigi.remove(process.simSiPixelDigis)
process.pdigi.remove(process.simEcalUnsuppressedDigis)
process.pdigi.remove(process.simEcalTriggerPrimitiveDigis)
process.pdigi.remove(process.simEcalDigis)
process.pdigi.remove(process.simEcalPreshowerDigis)
process.pdigi.remove(process.simHcalUnsuppressedDigis)
process.pdigi.remove(process.simHcalTriggerPrimitiveDigis)
process.pdigi.remove(process.simHcalDigis)
process.pdigi.remove(process.simHcalTTPDigis)
process.pdigi.remove(process.simCastorDigis)
process.pdigi.remove(process.simMuonCSCDigis)
process.pdigi.remove(process.simMuonDTDigis)
process.pdigi.remove(process.simMuonRPCDigis)
#process.pdigi.remove(process.mergedtruth)
# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.endjob_step,process.RECOSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

