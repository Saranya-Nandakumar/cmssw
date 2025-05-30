import FWCore.ParameterSet.Config as cms

hltInitialStepSeedTracksLST = cms.EDProducer(
    "TrackFromSeedProducer",
    src = cms.InputTag("hltInitialStepSeeds"),
    beamSpot = cms.InputTag("hltOnlineBeamSpot"),
    TTRHBuilder = cms.string("hltESPTTRHBuilderWithoutRefit")
)
