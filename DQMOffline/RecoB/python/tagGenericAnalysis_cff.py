import FWCore.ParameterSet.Config as cms

bTagGenericAnalysisBlock = cms.PSet(
    parameters = cms.PSet(
        discriminatorStart = cms.double(-0.01),
        nBinEffPur = cms.int32(200),
        # the constant b-efficiency for the differential plots versus pt and eta
        effBConst = cms.double(0.7),
        endEffPur = cms.double(1.005),
        discriminatorEnd = cms.double(1.011),
        startEffPur = cms.double(0.005)
    )
)

cTagGenericAnalysisBlock = cms.PSet(
    parameters = cms.PSet(
        discriminatorStart = cms.double(-0.01),
        nBinEffPur = cms.int32(200),
        # the constant c-efficiency for the differential plots versus pt and eta
        effBConst = cms.double(0.7),
        endEffPur = cms.double(1.005),
        discriminatorEnd = cms.double(1.011),
        startEffPur = cms.double(0.005)
    )
)

tauTagGenericAnalysisBlock = cms.PSet(
    parameters = cms.PSet(
        discriminatorStart = cms.double(-0.01),
        nBinEffPur = cms.int32(200),
        # the constant tau-efficiency for the differential plots versus pt and eta
        effBConst = cms.double(0.7),
        endEffPur = cms.double(1.005),
        discriminatorEnd = cms.double(1.011),
        startEffPur = cms.double(0.005)
    )
)

sTagGenericAnalysisBlock = cms.PSet(
    parameters = cms.PSet(
        discriminatorStart = cms.double(-0.01),
        nBinEffPur = cms.int32(200),
        # the constant s-efficiency for the differential plots versus pt and eta
        effBConst = cms.double(0.7),
        endEffPur = cms.double(1.005),
        discriminatorEnd = cms.double(1.011),
        startEffPur = cms.double(0.005)
    )
)

qgTagGenericAnalysisBlock = cms.PSet(
    parameters = cms.PSet(
        discriminatorStart = cms.double(-0.01),
        nBinEffPur = cms.int32(200),
        # the constant q-efficiency for the differential plots versus pt and eta
        effBConst = cms.double(0.7),
        endEffPur = cms.double(1.005),
        discriminatorEnd = cms.double(1.011),
        startEffPur = cms.double(0.005)
    )
)

# TrackProbability jetTag configuration
bTagProbabilityAnalysisBlock = cms.PSet(
    parameters = cms.PSet(
        discriminatorStart = cms.double(-0.025),
        nBinEffPur = cms.int32(200),
        # the constant b-efficiency for the differential plots versus pt and eta
        effBConst = cms.double(0.5),
        endEffPur = cms.double(1.005),
        discriminatorEnd = cms.double(2.525),
        startEffPur = cms.double(0.005)
    )
)

# TrackIP tag info configuration
bTagTrackIPAnalysisBlock = cms.PSet(
    parameters = cms.PSet(
        QualityPlots = cms.bool(False),
        endEffPur = cms.double(1.005),
        nBinEffPur = cms.int32(200),
        startEffPur = cms.double(0.005),
        LowerIPSBound = cms.double(-35.0),
        UpperIPSBound = cms.double(35.0),
        LowerIPBound = cms.double(-0.1),
        UpperIPBound = cms.double(0.1),
        LowerIPEBound = cms.double(0.0),
        UpperIPEBound = cms.double(0.04),
        NBinsIPS = cms.int32(100),
        NBinsIP = cms.int32(100),
        NBinsIPE = cms.int32(100),
        # IP histograms with more than 2 % outside the range above on the RelVals (b-decay tails, PU jets):
        # booked on +-WideIPBound / +-WideIPSBound with the same bin width
        WideRangeHistograms = cms.vstring('ip_3D', 'ip1_3D', 'ip2_3D', 'ip3_3D', 'ip_2D', 'ip1_2D', 'ip2_2D',
                                          'ips_3D', 'ips1_3D', 'ips2_3D', 'ips1_2D'),
        WideIPBound = cms.double(0.3),
        WideIPSBound = cms.double(100.0),
        MinDecayLength = cms.double(-9999.0),
        MaxDecayLength = cms.double(5.0),
        MinJetDistance = cms.double(0.0),
        MaxJetDistance = cms.double(0.07),
    )
)
