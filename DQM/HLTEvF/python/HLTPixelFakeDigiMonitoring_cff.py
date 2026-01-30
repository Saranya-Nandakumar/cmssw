import FWCore.ParameterSet.Config as cms

# Import the fake digi monitor configured for HLT
from DQM.HLTEvF.SiPixelFakeDigiMonitor_cfi import SiPixelFakeDigiMonitorHLT

# Uses SoA clusters which contain the nFakeDigis column
hltPixelFakeDigiMonitor = SiPixelFakeDigiMonitorHLT.clone(
    topFolderName = cms.string("HLT/Pixel/FakeDigis"),
    clustersSoASrc = cms.InputTag("hltSiPixelClustersAlpaka"),  
    digisSoASrc = cms.InputTag("hltSiPixelDigisSoAAlpaka"),  
)

# Sequence for HLT DQM
hltPixelFakeDigiMonitoringSequence = cms.Sequence(
    hltPixelFakeDigiMonitor
)
