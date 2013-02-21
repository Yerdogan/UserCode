import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
#process.MessageLogger.cerr.threshold = 'INFO'
#process.MessageLogger.detailedInfo.eventNumber.reportEvery = cms.untracked.PSet(100)
#process.MessageLogger.categories.append('Demo')
#process.MessageLogger.cerr.INFO = cms.untracked.PSet(
#    limit=cms.untracked.int32(-1)
#)

#HBHE event-level noise filtering
process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')

#process.load('Configuration.Geometry.GeometryAll_cff')
#process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load('Configuration.Geometry.GeometryIdeal_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load('Configuration/StandardSequences/Reconstruction_cff')

process.GlobalTag.globaltag = 'FT_R_53_V6::All'

#process.load("Calibration.HcalAlCaRecoProducers.alcahomuon_cfi")
#process.load("RecoLocalCalo.HcalRecAlgos.hcalRecAlgoESProd_cfi")

#FileService for histograms
process.TFileService = cms.Service("TFileService",
    fileName=cms.string(
        'SingleMu_Run2012A_13Jul2012_v1_RECO.root'
        #'/user/erdogan/output/SingleMu_Run2012A_13Jul2012_v1_RECO_2.root'
        #'/user/erdogan/output/SingleMu_Run2012A_13Jul2012_v1_RECO_3.root'
    )
)

process.options = cms.untracked.PSet(wantSummary=cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(-1))

process.source = cms.Source("PoolSource",
    fileNames=cms.untracked.vstring(
        #'file:/user/erdogan/samples/SingleMu_RECO_13Jul2012-v1_00001_46BAC80C-58D0-E111-85E9-1CC1DE046FC0.root'
        #'file:/user/erdogan/samples/SingleMu_RECO_13Jul2012-v1_00001_AE363203-55D0-E111-A7ED-1CC1DE1D16D4.root'             
        #'file:/user/erdogan/samples/SingleMu_RECO_13Jul2012-v1_00001_DA41A7D9-39D2-E111-A7D9-1CC1DE0530F8.root'
    )
)

process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
JSONfile = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-203853_8TeV_PromptReco_Collisions12_JSON_v2.txt'
myLumis = LumiList.LumiList(filename=JSONfile).getCMSSWString().split(',')
process.source.lumisToProcess.extend(myLumis)

#Beam background removal
process.noscraping = cms.EDFilter("FilterOutScraping",
                                applyfilter=cms.untracked.bool(True),
                                debugOn=cms.untracked.bool(False),
                                numtrack=cms.untracked.uint32(10),
                                thresh=cms.untracked.double(0.25)
                                )

#Primary vertex requirement
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection=cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF=cms.uint32(4) ,
                                           maxAbsZ=cms.double(24),
                                           maxd0=cms.double(2) 
                                           )

process.MuonSelector = cms.EDFilter("MuonSelector",
                                     src=cms.InputTag("muons"),
                                     cut=cms.string("isGlobalMuon && eta < 1.32 && eta > -1.32 && pt > 50")
                                     )

process.demo = cms.EDAnalyzer('HOEfficiencyAnalyzer',
    selection=cms.string("isGlobalMuon"),
    beamSpot=cms.InputTag("offlineBeamSpot"),
    primaryVertex=cms.InputTag('offlinePrimaryVertices')        
)

process.p = cms.Path(process.noscraping
                     * process.primaryVertexFilter 
                     * process.MuonSelector
                     * process.demo)

