import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("Fullsim")

## Stack separation in mm
##sep = '1'
# Number of PU events
npu = '0'
# Number of events to process
nevents = 500;
# PID of gun particle
# NOTE there are 4 fired each time
# AntiParticle is set this way:
# Pt and Eta range of gun particle

# Define Algorithms:   Clusters,  Stubs,            Tracklets,         Tracks
ObjectsFromSimHits = [ "2d" ,     "globalgeometry", "globalgeometry" , "bpphel" ]
ObjectsFromDigis   = [ "2d" ,     "pixelray",       "globalgeometry" , "bpphel" ]
# Set Stub production pt threshold
thresh = 2.0
# Set Tracklet flags
trackletonlyherm = cms.bool(True) # True creates only hermetic-FNAL style Tracklets
tracklethelixfit = cms.bool(True) # True fits the Tracklet with helicoidal style in R-Z, creating dedicated collection
trackletbeamspot = cms.bool(True) # True corrects the Tracklet fit for the beam position, creating dedicated collection
# Set Track flags
trackletseedtype = "ShortTrackletsVtxBSHelFit" # Recommended because the only algorithm available is bpphel!
# NOTE be self consistent with available seeds
# and with available fit strategies
trackhelixfit = cms.bool(True) # True applies helicoidal fit to Tracks
trackalsoseed = cms.bool(True) # True includes seed vertex within points used for the fit
tracknoduplic = cms.bool(True) # True removes duplicate candidate Tracks
# NOTE only one collection is created by the L1TkTrackBuilder
trackdsvector = cms.vuint32(  ) # empty to allow all of them



index = 1
puindex = 111
if os.getenv('pgun_index'):
   index = str(os.getenv('pgun_index'))
   puindex = int(os.getenv('pgun_index'))

theSeedValue = 12445
if os.getenv('pgun_index'):
   theSeedValue = int(1255+1000*int(os.getenv('pgun_index'))*int(os.getenv('pgun_index')))
print 'Seed: '+str(theSeedValue)

process.load("Configuration.StandardSequences.Generator_cff")
process.load("Configuration.StandardSequences.Simulation_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load('SLHCUpgradeSimulations.Geometry.LongBarrel_cmsSimIdealGeometryXML_cff')
process.load("SLHCUpgradeSimulations.Utilities.StackedTrackerGeometry_cfi")
process.load('SLHCUpgradeSimulations.Geometry.fakeConditions_LongBarrel_cff')
process.load("SLHCUpgradeSimulations.Geometry.recoFromSimDigis_LongBarrel_cff")
#process.load("SLHCUpgradeSimulations.Geometry.upgradeTracking_longbarrel_cff")

process.XMLIdealGeometryESSource.geomXMLFiles.append('MTTStudies/Geometry/data/mttalgo_1Layer.xml')
process.XMLIdealGeometryESSource.geomXMLFiles.append('MTTStudies/Geometry/data/mttsens_1Layer.xml')
process.XMLIdealGeometryESSource.geomXMLFiles.append('MTTStudies/Geometry/data/mttNumbering.xml')

## choose magnetic field
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
#process.load("Configuration.StandardSequences.MagneticField_40T_cff")

process.load('Configuration/StandardSequences/Sim_cff')
process.load('Configuration/StandardSequences/Digi_cff')
process.load("MTTStudies.Geometry.mttGeometry_cfi")
process.load("MTTStudies.MTTDigiProducer.mttdigitizer_cfi")
process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")

process.load('Configuration/StandardSequences/SimL1Emulator_cff')
process.load('Configuration/StandardSequences/DigiToRaw_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')

process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "DESIGN42_V11::All"
process.TrackerDigiGeometryESModule.applyAlignment = False

## choose random number generators and seeds
process.load('IOMC/RandomEngine/IOMC_cff')
#process.RandomNumberGeneratorService.generator.engineName = cms.untracked.string('TRandom3')
#process.RandomNumberGeneratorService.VtxSmeared.engineName = cms.untracked.string('TRandom3')
#process.RandomNumberGeneratorService.g4SimHits.engineName = cms.untracked.string('TRandom3')
#process.RandomNumberGeneratorService.mix.engineName = cms.untracked.string('TRandom3')
#process.RandomNumberGeneratorService.LHCTransport.engineName = cms.untracked.string('TRandom3')
#process.RandomNumberGeneratorService.simSiPixelDigis.engineName = cms.untracked.string('TRandom3')

process.RandomNumberGeneratorService.generator.initialSeed = theSeedValue+111211
process.RandomNumberGeneratorService.VtxSmeared.initialSeed=theSeedValue+22222
process.RandomNumberGeneratorService.g4SimHits.initialSeed=theSeedValue+33333
process.RandomNumberGeneratorService.mix.initialSeed=theSeedValue+33333
process.RandomNumberGeneratorService.LHCTransport.initialSeed=theSeedValue+33333
process.RandomNumberGeneratorService.simSiPixelDigis.initialSeed=theSeedValue+33333

process.RandomNumberGeneratorService.simMuonMTTDigis = cms.PSet(
    initialSeed = cms.untracked.uint32(970)
)

from IOMC.RandomEngine.RandomServiceHelper import  RandomNumberServiceHelper
randHelper = RandomNumberServiceHelper(process.RandomNumberGeneratorService)
randHelper.populate()

process.g4SimHits.MTTSD=cms.PSet(EnergyThresholdForHistoryInGeV = cms.double(0.05),EnergyThresholdForPersistencyInGeV = cms.double(0.1))
#modify tracking particles to include mtt
#process.mergedtruth.simHitCollections.muon.append("g4SimHitsMTTHits")
#dito with mixing module


# replace with long barrel geometry
#MOVED UPSTAIRS sep = '1'
##if os.getenv('pgun_sep'):
##   sep= str(os.getenv('pgun_sep'))
##print 'Stack Separation: '+sep

process.siPixelFakeGainOfflineESSource = cms.ESSource("SiPixelFakeGainOfflineESSource",
        file = cms.FileInPath('SLHCUpgradeSimulations/Geometry/data/LongBarrel/PixelSkimmedGeometry_empty.txt'))
process.es_prefer_fake_gain = cms.ESPrefer("SiPixelFakeGainOfflineESSource","siPixelFakeGainOfflineESSource")
process.siPixelFakeLorentzAngleESSource = cms.ESSource("SiPixelFakeLorentzAngleESSource",
        file = cms.FileInPath('SLHCUpgradeSimulations/Geometry/data/LongBarrel/PixelSkimmedGeometry.txt'))
process.es_prefer_fake_lorentz = cms.ESPrefer("SiPixelFakeLorentzAngleESSource","siPixelFakeLorentzAngleESSource")

#MOVED UPSTAIRS npu = '0'
if os.getenv('pgun_npu'):
   npu= str(os.getenv('pgun_npu'))
print 'Number of PU events: '+npu

################################################################################
# MixingModule config
#   load properly pre-preprocessed minbias for pileup
#   use only relevant simhit collections for mixing
#   do not mix simtracks or simvertices so far

# minbias files for pileup simulated in 1/2 of detector

files_MB4PU_2pi = cms.untracked.vstring(
"""
    'dcap://cmsgridftp.fnal.gov:24125/pnfs/fnal.gov/usr/cms/WAX/11/store/user/salvati/FullSim/363/PileUp/MinBias__GEN_SIM-01_363.root',
"""
    )

# files_MB4PU is later feeded into mix.input.fileNames
files_MB4PU = files_MB4PU_2pi

from SimGeneral.MixingModule.mixObjects_cfi import *
process.mix = cms.EDProducer("MixingModule",
        LabelPlayback = cms.string(''),
        playback = cms.untracked.bool(False),
#        maxBunch = cms.int32(3),  ## in terms of 25 ns
#        minBunch = cms.int32(-5), ## in terms of 25 ns
        maxBunch = cms.int32(0),  ## turning off OOT pileup to compare with fastsim
        minBunch = cms.int32(0),                           
        bunchspace = cms.int32(50), ## ns
        mixProdStep1 = cms.bool(False),
        mixProdStep2 = cms.bool(False),
        useCurrentProcessOnly = cms.bool(False),
        input = cms.SecSource("PoolSource",
                    nbPileupEvents = cms.PSet( averageNumber = cms.double(npu) ),
                    seed = cms.int32(12345+1000*puindex),
                    type = cms.string('poisson'), # ('fixed'),
                    sequential = cms.untracked.bool(False),
#                    inputCommands = cms.untracked.vstring('keep *'),
#                    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
                    fileNames = files_MB4PU
        ),
        mixObjects = cms.PSet(mixCH = cms.PSet( mixCaloHits ),
                         mixTracks = cms.PSet( mixSimTracks ),
                         mixVertices = cms.PSet( mixSimVertices ),
                         mixSH = cms.PSet( mixSimHits ),
                         mixHepMC = cms.PSet( mixHepMCProducts )
        )
)

#dito with mixing module
#process.mixSimHits.input.append(cms.InputTag("g4SimHits","MTTHits"))
#process.mixSimHits.subdets.append("MTTHits")
process.mix.mixObjects.mixSH.subdets.append('MTTHits')
process.mix.mixObjects.mixSH.input.append(cms.InputTag("g4SimHits","MTTHits"))


#process.load("Configuration.StandardSequences.L1Emulator_cff")
#process.load("Configuration.StandardSequences.DigiToRaw_cff")
#process.load("Configuration.StandardSequences.RawToDigi_cff")

# Re-set the beamspot scenario here! 
#process.load("Configuration.StandardSequences.VtxSmeared")
process.load("IOMC.EventVertexGenerators.VtxSmearedGauss_cfi")
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load("SimTracker.Configuration.SimTracker_cff")
#process.simSiPixelDigis.DeltaProductionCut = cms.double(100000); # Will turn off delta rays?

process.simSiPixelDigis.ROUList = cms.vstring(
    'g4SimHitsTrackerHitsPixelBarrelLowTof','g4SimHitsTrackerHitsPixelBarrelHighTof',
    'g4SimHitsTrackerHitsPixelEndcapLowTof','g4SimHitsTrackerHitsPixelEndcapHighTof')
process.simSiPixelDigis.MissCalibrate = False
process.simSiPixelDigis.LorentzAngle_DB = False
process.simSiPixelDigis.killModules = False
process.simSiPixelDigis.NumPixelBarrel = cms.int32(14)
process.simSiPixelDigis.NumPixelEndcap = cms.int32(3)
process.simSiPixelDigis.AddPixelInefficiency = -1
process.simSiStripDigis.ROUList = cms.vstring(
    'g4SimHitsTrackerHitsPixelBarrelLowTof','g4SimHitsTrackerHitsPixelBarrelHighTof',
    'g4SimHitsTrackerHitsPixelEndcapLowTof','g4SimHitsTrackerHitsPixelEndcapHighTof')
process.siPixelClusters.src = 'simSiPixelDigis'
process.siPixelClusters.MissCalibrate = False
"""
tracker parameters defined in the configuration file :
	double ElectronPerAdc=135.0           - calibration electrons/adc
        double NoiseInElectrons =500.0        - pixel noise in electrons
        double ThresholdInNoiseUnits = 5.     - pixel threshold in noise   
        double TanLorentzAnglePerTesla =0.106 - Lorentz angle per unit B field
        int32 AddPixelInefficiency = 0        - pixel inefficiency 
                                               -1 - none
                                                0 - static loss only, v.low lumi
                                                2 - low lumi
                                               10 - high lumi    
        bool AddNoise = true                  - add noise to the pixel signal 
        bool AddNoisyPixels = true            - add pixels generated by noise 
        bool MissCalibrate = false            - misscalibration the pixel gain 
        double GainSmearing = 0.0             - misscalibration spread in gain
        double OffsetSmearing = 0.0           - misscalibration spread in offset

untracked parameters with default values provided :
	int AdcFullScale  = 255               - adc full scale (8 bits)
	double TofCut     = 12.5              - time of flight cut in nanoseconds
	bool FluctuateCharge = true           - fluctuate generated charge along the track 
        double DeltaProductionCut = 0.030     - the delta production cut in MeV, 
                                              (has to be equal to the value used in OSCAR)
"""






# Event output
process.load("Configuration.EventContent.EventContent_cff")

#MOVEDUPSTAIRS nevents = 2000;
if os.getenv('pgun_nevents'):
   nevents = int(os.getenv('pgun_nevents'))
print 'Generating '+str(nevents)+' events.'
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(nevents)
)


#######################################################################################################################
## SLHC Track-trigger primitives
#######################################################################################################################

# HERE YOU CAN OVERRIDE DEFAULTS!
process.load("SLHCUpgradeSimulations.L1TrackTrigger.ClusteringAlgorithmRegister_cfi")
process.load("SLHCUpgradeSimulations.L1TrackTrigger.HitMatchingAlgorithmRegister_cfi")
process.load("SLHCUpgradeSimulations.L1TrackTrigger.StubPairingAlgorithmRegister_cfi")
process.load("SLHCUpgradeSimulations.L1TrackTrigger.TrackingAlgorithmRegister_cfi")

process.ClusteringAlgorithm_PSimHit_   = cms.ESPrefer("ClusteringAlgorithm_"+ObjectsFromSimHits[0]+"_PSimHit_")
process.ClusteringAlgorithm_PixelDigi_ = cms.ESPrefer("ClusteringAlgorithm_"+ObjectsFromDigis[0]+"_PixelDigi_")

process.HitMatchingAlgorithm_PSimHit_   = cms.ESPrefer("HitMatchingAlgorithm_"+ObjectsFromSimHits[1]+"_PSimHit_")
process.HitMatchingAlgorithm_PixelDigi_ = cms.ESPrefer("HitMatchingAlgorithm_"+ObjectsFromDigis[1]+"_PixelDigi_")

process.StubPairingAlgorithm_PSimHit_   = cms.ESPrefer("StubPairingAlgorithm_"+ObjectsFromSimHits[2]+"_PSimHit_")
process.StubPairingAlgorithm_PixelDigi_ = cms.ESPrefer("StubPairingAlgorithm_"+ObjectsFromDigis[2]+"_PixelDigi_")

process.TrackingAlgorithm_PSimHit_   = cms.ESPrefer("TrackingAlgorithm_"+ObjectsFromSimHits[3]+"_PSimHit_")
process.TrackingAlgorithm_PixelDigi_ = cms.ESPrefer("TrackingAlgorithm_"+ObjectsFromDigis[3]+"_PixelDigi_")
# Define input types
process.load("SLHCUpgradeSimulations.L1TrackTrigger.Beam_cfi")
process.load("SLHCUpgradeSimulations.L1TrackTrigger.Cluster_cfi")
process.load("SLHCUpgradeSimulations.L1TrackTrigger.Stub_cfi")
process.load("SLHCUpgradeSimulations.L1TrackTrigger.Tracklet_cfi")
process.load("SLHCUpgradeSimulations.L1TrackTrigger.Track_cfi")

process.L1TkClustersFromPixelDigis.rawHits = cms.VInputTag(cms.InputTag("simSiPixelDigis"))
process.L1TkStubsFromPixelDigis.L1TkClusters = cms.InputTag("L1TkClustersFromPixelDigis")
process.L1TkTrackletsFromPixelDigis.L1TkStubs = cms.InputTag("L1TkStubsFromPixelDigis")
process.L1TkTrackletsFromPixelDigis.CreateOnlyHermetic = trackletonlyherm
process.L1TkTrackletsFromPixelDigis.DoHelixFit = tracklethelixfit
process.L1TkTrackletsFromPixelDigis.UseBeamspotVertex = trackletbeamspot
process.L1TkTracksFromPixelDigis.L1TkStubsBricks = cms.InputTag("L1TkStubsFromPixelDigis")
# be selfconsistent with the use of HelixFit in both seed and track
process.L1TkTracksFromPixelDigis.L1TkTrackletSeed = cms.InputTag("L1TkTrackletsFromPixelDigis",trackletseedtype)
process.L1TkTracksFromPixelDigis.DoHelixFit = trackhelixfit
process.L1TkTracksFromPixelDigis.UseAlsoSeedVertex = trackalsoseed
process.L1TkTracksFromPixelDigis.SeedDoubleStacks = trackdsvector
process.L1TkTracksFromPixelDigis.RemoveDuplicates = tracknoduplic
   


# Set stub production pt threshold
if os.getenv('pgun_thresh'):
    thresh = float(os.getenv('pgun_thresh'))
print 'Using stub production threshold: '+str(thresh)+" GeV"
process.HitMatchingAlgorithm_globalgeometry_PSimHit_.minPtThreshold = thresh
process.HitMatchingAlgorithm_globalgeometry_PixelDigi_.minPtThreshold = thresh
process.HitMatchingAlgorithm_pixelray_PixelDigi_.minPtThreshold = thresh

#######################################################################################################################

process.load("SimTracker.TrackAssociation.TrackAssociatorByChi2_cfi")
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")

"""
process.load("Validation.RecoTrack.MultiTrackValidator_cff")
#process.multiTrackValidator.label = ['generalTracks']
### if using simple (non-iterative) or old (as in 1_8_4) tracking
process.multiTrackValidator.label = ['ctfWithMaterialTracks']
process.multiTrackValidator.associators = ['TrackAssociatorByHits']
process.multiTrackValidator.UseAssociators = True
process.multiTrackValidator.outputFile = "validfullLongBarrel_muon_50GeV.root"
process.multiTrackValidator.nint = cms.int32(20)
process.multiTrackValidator.nintpT = cms.int32(25)
process.multiTrackValidator.maxpT = cms.double(50.0)
"""

##### with John's changes ##############################
process.load("SLHCUpgradeSimulations.Geometry.oldTracking_wtriplets")
process.pixellayertriplets.layerList = cms.vstring('BPix1+BPix2+BPix3',
        'BPix1+BPix3+BPix4',
        'BPix2+BPix3+BPix4',
        'BPix1+BPix2+BPix4',
        'BPix1+BPix2+FPix1_pos',
        'BPix1+BPix2+FPix1_neg',
        'BPix1+FPix1_pos+FPix2_pos',
        'BPix1+FPix1_neg+FPix2_neg',
        'BPix1+FPix2_pos+FPix3_pos',
        'BPix1+FPix2_neg+FPix3_neg',
        'FPix1_pos+FPix2_pos+FPix3_pos',
        'FPix1_neg+FPix2_neg+FPix3_neg')


process.load("Validation.RecoTrack.cutsTPEffic_cfi")
process.load("Validation.RecoTrack.cutsTPFake_cfi")


# restrict vertex fining in trackingtruthprod to smaller volume (note: these numbers in mm)
process.mergedtruth.simHitLabel = 'g4SimHits'
process.mergedtruth.volumeRadius = cms.double(100.0)
process.mergedtruth.volumeZ = cms.double(900.0)
process.mergedtruth.discardOutVolume = cms.bool(True)
process.mergedtruth.simHitCollections = cms.PSet(
    pixel = cms.vstring('g4SimHitsTrackerHitsPixelBarrelLowTof',
                        'g4SimHitsTrackerHitsPixelBarrelHighTof',
                        'g4SimHitsTrackerHitsPixelEndcapLowTof',
                        'g4SimHitsTrackerHitsPixelEndcapHighTof')
    )


process.cutsTPEffic.ptMin = cms.double(1.8)
process.cutsTPEffic.minRapidity = cms.double(-20.0)
process.cutsTPEffic.maxRapidity = cms.double(20.0)
process.cutsTPEffic.signalOnly = cms.bool(False)
process.cutsTPEffic.stableOnly = cms.bool(False)
process.cutsTPEffic.chargedOnly = cms.bool(False)


process.cutsTPFake.ptMin = cms.double(1.8)
process.cutsTPFake.minRapidity = cms.double(-20.0)
process.cutsTPFake.maxRapidity = cms.double(20.0)
process.cutsTPFake.signalOnly = cms.bool(False)
process.cutsTPFake.stableOnly = cms.bool(False)
process.cutsTPFake.chargedOnly = cms.bool(False)
#process.cutsTPFake.tip = cms.double(10.0)
#process.cutsTPFake.lip = cms.double(90.0)
############ end John's changes ###########################

### make sure the correct (modified) error routine is used
process.siPixelRecHits.CPE = 'PixelCPEfromTrackAngle'
process.MeasurementTracker.PixelCPE = 'PixelCPEfromTrackAngle'
process.ttrhbwr.PixelCPE = 'PixelCPEfromTrackAngle'
process.mixedlayerpairs.BPix.TTRHBuilder = cms.string('WithTrackAngle')
process.mixedlayerpairs.FPix.TTRHBuilder = cms.string('WithTrackAngle')
process.pixellayertriplets.BPix.TTRHBuilder = cms.string('WithTrackAngle')
process.pixellayertriplets.FPix.TTRHBuilder = cms.string('WithTrackAngle')
process.ctfWithMaterialTracks.TTRHBuilder = cms.string('WithTrackAngle')
#next may not be needed
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.TTRHBuilder = cms.string('WithTrackAngle')

#next may not be needed
process.load("RecoTracker.SiTrackerMRHTools.SiTrackerMultiRecHitUpdator_cff")
process.siTrackerMultiRecHitUpdator.TTRHBuilder = cms.string('WithTrackAngle')

"""
#replace with correct component in cloned version (replace with original TTRH producer)
#process.preFilterFirstStepTracks.TTRHBuilder = cms.string('WithTrackAngle')
#process.secPixelRecHits.CPE = cms.string('PixelCPEfromTrackAngle')
process.seclayertriplets.BPix.TTRHBuilder = cms.string('WithTrackAngle')
process.seclayertriplets.FPix.TTRHBuilder = cms.string('WithTrackAngle')
process.secMeasurementTracker.PixelCPE = cms.string('PixelCPEfromTrackAngle')
process.secWithMaterialTracks.TTRHBuilder = cms.string('WithTrackAngle')
process.thlayerpairs.BPix.TTRHBuilder = cms.string('WithTrackAngle')
process.thlayerpairs.FPix.TTRHBuilder = cms.string('WithTrackAngle')
process.thMeasurementTracker.PixelCPE = cms.string('PixelCPEfromTrackAngle')
process.thWithMaterialTracks.TTRHBuilder = cms.string('WithTrackAngle')
"""

"""
process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("detailedInfo_FullSim")
process.Timing =  cms.Service("Timing")
"""

process.source = cms.Source("EmptySource")

process.generator = cms.EDFilter("Pythia6GeneratorFilter",
                                    pythiaPylistVerbosity = cms.untracked.int32(0),
                                    filterEfficiency = cms.untracked.double(1.0),
                                    pythiaHepMCVerbosity = cms.untracked.bool(False),
                                    comEnergy = cms.double(14000.0),
                                    maxEventsToPrint = cms.untracked.int32(0),
                                    PythiaParameters = cms.PSet(
           pythiaUESettings = cms.vstring('MSTJ(11)=3     ! Choice of the fragmentation function',
                                                     'MSTJ(22)=2     ! Decay those unstable particles',
                                                     'PARJ(71)=10 .  ! for which ctau  10 mm',
                                                     'MSTP(2)=1      ! which order running alphaS',
                                                     'MSTP(33)=0     ! no K factors in hard cross sections',
                                                     'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)',
                                                     'MSTP(52)=2     ! work with LHAPDF',
                                                     'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default',
                                                     'MSTP(82)=4     ! Defines the multi-parton model',
                                                     'MSTU(21)=1     ! Check on possible errors during program execution',
                                                     'PARP(82)=1.8387   ! pt cutoff for multiparton interactions',
                                                     'PARP(89)=1960. ! sqrts for which PARP82 is set',
                                                     'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter',
                                                     'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter',
                                                     'PARP(90)=0.16  ! Multiple interactions: rescaling power',
                                                     'PARP(67)=2.5    ! amount of initial-state radiation',
                                                     'PARP(85)=1.0  ! gluon prod. mechanism in MI',
                                                     'PARP(86)=1.0  ! gluon prod. mechanism in MI',
                                                     'PARP(62)=1.25   ! ',
                                                     'PARP(64)=0.2    ! ',
                                                     'MSTP(91)=1      !',
                                                     'PARP(91)=2.1   ! kt distribution',
                                                     'PARP(93)=15.0  ! '),
                  processParameters = cms.vstring('MSEL=0         ! User defined processes',
                                                             'MSUB(11)=1     ! Min bias process',
                                                             'MSUB(12)=1     ! Min bias process',
                                                             'MSUB(13)=1     ! Min bias process',
                                                             'MSUB(28)=1     ! Min bias process',
                                                             'MSUB(53)=1     ! Min bias process',
                                                             'MSUB(68)=1     ! Min bias process',
                                                             'MSUB(92)=1     ! Min bias process, single diffractive',
                                                             'MSUB(93)=1     ! Min bias process, single diffractive',
                                                             'MSUB(94)=1     ! Min bias process, double diffractive',
                                                             'MSUB(95)=1     ! Min bias process'),
                  parameterSets = cms.vstring('pythiaUESettings',
                                                         'processParameters')
              )
                                 )

process.o1 = cms.OutputModule("PoolOutputModule",
# definition of output file (full path)
    fileName = cms.untracked.string('FullSim_withMTT10Tile2Strip_MinBias_TuneZ2star_14TeV.root'),
    #cms.untracked.string('FullSim_N'+str(nevents)+'_PID'+str(pid)+'_PT'+str(plow)+'_'+str(phigh)+'_ETA'+str(elow)+'_'+str(ehigh)+'_PU'+str(npu)+'_thresh'+str(thresh)+'GeV_'+str(index)+'_PixelRay_50ns_NOOOT.root'),
    outputCommands = cms.untracked.vstring('drop *',
###########################################################################################
#                                           'keep PSimHit*_*_*_*',
###########################################################################################
                                           'keep *_L1TkBeams_*_*',
                                           'keep *_simMuonMTTDigis_*_*',
                                           'keep *_*MTT*_*_*',
                                           'keep *_L1TkClustersFromPixelDigis_*_*',
                                           'keep *_L1TkStubsFromPixelDigis_*_*',
                                           'keep *_L1TkTrackletsFromPixelDigis_*_*',
                                           'keep *_L1TkTracksFromPixelDigis_*_*',
###########################################################################################
                                           'keep *_L1TkClustersFromSimHits_*_*',
                                           'keep *_L1TkStubsFromSimHits_*_*',
                                           'keep *_L1TkTrackletsFromSimHits_*_*',
                                           'keep *_L1TkTracksFromSimHits_*_*',
###########################################################################################
#                                           'keep *_famosSimHits_*_*',
                                           'keep PSimHits_g4SimHits_*_*',
                                           'keep SimTracks_g4SimHits_*_*',
                                           'keep SimVertexs_g4SimHits_*_*',
#                                           'keep *_ctfWithMaterialTracks_*_*',
                                           'keep *_simSiPixelDigis_*_*',
#                                           'keep *_simSiStripDigis_*_*',
#                                           'keep *_siStripMatchedRecHits_*_*',
#                                           'keep *_siPixelRecHits_*_*',
#                                           'keep *_siPixelClusters_*_*',
#                                           'keep *_siStripClusters_*_*',
#                                           'keep *_siPixelDigis_*_*',
#                                           'keep *_siStripDigis_*_*',
#                                           'keep *_L1ExtraMaker_*_*',
                                           'keep *_genParticles_*_*', # montecarlo tree
#                                           'keep *_offlineBeamSpot_*_*',
                                          )
)
process.outpath = cms.EndPath(process.o1)
process.analyze = cms.EDAnalyzer('EventContentAnalyzer')

process.g4SimHits.Generator.MinEtaCut = -5.5
process.g4SimHits.Generator.MaxEtaCut = 5.5
process.g4SimHits.Generator.MinPCut = 0.

process.p0 = cms.Path(process.generator*process.VtxSmeared*process.genParticles)
process.p1 = cms.Path(process.psim)
process.p2 = cms.Path(process.randomEngineStateProducer*process.mix*process.simSiPixelDigis+process.simMuonMTTDigis)
process.p3 = cms.Path(process.siPixelClusters)
process.p4 = cms.Path(process.L1TkBeams*process.L1TkClustersFromPixelDigis*process.L1TkStubsFromPixelDigis*process.L1TkTrackletsFromPixelDigis*process.L1TkTracksFromPixelDigis)
process.p5 = cms.Path(process.analyze)
#process.schedule = cms.Schedule(process.p0,process.p1,process.p2,process.p3,process.p4,process.p5,process.outpath)
#process.schedule = cms.Schedule(process.p0,process.p1,process.p2,process.p3,process.p4,process.outpath)
process.schedule = cms.Schedule(process.p0,process.p1,process.outpath)

