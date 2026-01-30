import FWCore.ParameterSet.Config as cms

# SiPixel Fake Digi Monitor
from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer

SiPixelFakeDigiMonitor = DQMEDAnalyzer('SiPixelFakeDigiMonitor',
    clustersSoASrc = cms.InputTag('siPixelClustersPreSplittingAlpaka'),  # SoA with fake digi counts
    digisSoASrc = cms.InputTag('siPixelDigisSoAAlpaka'),  # SoA for moduleId -> DetId mapping
    digiSrc = cms.InputTag('siPixelDigis'),

    # For HLT use: 'HLT/Pixel'
    # For offline use: 'Pixel'
    topFolderName = cms.string('Pixel'),
)

# HLT-specific configuration
SiPixelFakeDigiMonitorHLT = SiPixelFakeDigiMonitor.clone(
    topFolderName = cms.string('HLT/Pixel'),
    clustersSoASrc = cms.InputTag('hltSiPixelClustersAlpaka'),
    digisSoASrc = cms.InputTag('hltSiPixelDigisSoAAlpaka'),
    digiSrc = cms.InputTag('hltSiPixelDigis'),
)
