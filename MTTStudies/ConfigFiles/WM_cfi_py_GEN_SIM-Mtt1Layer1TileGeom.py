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
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('MTTStudies.Geometry.cmsMttextendedGeom_1Layer_cfi')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedGauss_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.g4SimHits.MTTSD=cms.PSet(EnergyThresholdForHistoryInGeV = cms.double(0.001),EnergyThresholdForPersistencyInGeV = cms.double(0.01))
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
#process.Tracer = cms.Service("Tracer") 
# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('W->mu nu'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('WM_14TeV_pythia6_cfi_py_GEN_SIM-Mtt1Layer1TileGeom.root'),
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

from Configuration.Generator.PythiaUESettings_cfi import *
process.generator = cms.EDFilter("Pythia6GeneratorFilter",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(1.0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    crossSection = cms.untracked.double(17120.0),
    comEnergy = cms.double(14000.0),
    maxEventsToPrint = cms.untracked.int32(0),
    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        processParameters = cms.vstring('MSEL        = 0    !User defined processes', 
            'MSUB(2)     = 1    !W production', 
            'MDME(190,1) = 0    !W decay into dbar u', 
            'MDME(191,1) = 0    !W decay into dbar c', 
            'MDME(192,1) = 0    !W decay into dbar t', 
            'MDME(194,1) = 0    !W decay into sbar u', 
            'MDME(195,1) = 0    !W decay into sbar c', 
            'MDME(196,1) = 0    !W decay into sbar t', 
            'MDME(198,1) = 0    !W decay into bbar u', 
            'MDME(199,1) = 0    !W decay into bbar c', 
            'MDME(200,1) = 0    !W decay into bbar t', 
            'MDME(206,1) = 0    !W decay into e+ nu_e', 
            'MDME(207,1) = 1    !W decay into mu+ nu_mu', 
            'MDME(208,1) = 0    !W decay into tau+ nu_tau'),
        # This is a vector of ParameterSet names to be read, in this order
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters')
    )
)

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)
#process.RAWSIMoutput
# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

