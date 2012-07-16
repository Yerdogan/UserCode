import FWCore.ParameterSet.Config as cms

simMuonMTTDigis = cms.EDProducer('MTTDigitizer',
    digiModelConfig = cms.PSet(
#        signalPropagationSpeed = cms.double(0.66),
    ),
    InputCollection = cms.string('g4SimHitsMTTHits'),
    digiModel = cms.string('MTTSimTriv')
)
