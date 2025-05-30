import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQMEDHarvester import DQMEDHarvester

BTVEfficiency_BTagMu_DiJet = DQMEDHarvester("DQMGenericClient",
    subDirs        = cms.untracked.vstring("HLT/BTV/BTagMu_DiJet/*"),
    verbose        = cms.untracked.uint32(0), # Set to 2 for all messages
    resolution     = cms.vstring(),
    efficiency     = cms.vstring(
        "effic_met       'efficiency vs MET;MET [GeV];efficiency' met_numerator       met_denominator",
        "effic_metPhi       'efficiency vs MET #phi;MET #phi;efficiency' metPhi_numerator       metPhi_denominator",
        "effic_muPt_1       'efficiency vs muon pt; muon pt [GeV]; efficiency' muPt_1_numerator       muPt_1_denominator",
        "effic_muEta_1       'efficiency vs muon eta; muon eta ; efficiency' muEta_1_numerator       muEta_1_denominator",
        "effic_muPhi_1       'efficiency vs muon phi; muon phi ; efficiency' muPhi_1_numerator       muPhi_1_denominator",
        "effic_jetPt_1       'efficiency vs leading jet pt; jet pt [GeV]; efficiency' jetPt_1_numerator       jetPt_1_denominator",
        "effic_jetEta_1       'efficiency vs leading jet eta; jet eta ; efficiency' jetEta_1_numerator       jetEta_1_denominator",
        "effic_jetPhi_1       'efficiency vs leading jet phi; jet phi ; efficiency' jetPhi_1_numerator       jetPhi_1_denominator",
        "effic_jetPt_2       'efficiency vs leading jet pt; jet pt [GeV]; efficiency' jetPt_2_numerator       jetPt_2_denominator",
        "effic_jetEta_2       'efficiency vs leading jet eta; jet eta ; efficiency' jetEta_2_numerator       jetEta_2_denominator",
        "effic_jetPhi_2       'efficiency vs leading jet phi; jet phi ; efficiency' jetPhi_2_numerator       jetPhi_2_denominator",
        "effic_eventHT       'efficiency vs event HT; event HT [GeV]; efficiency' eventHT_numerator       eventHT_denominator",
        "effic_jetEtaPhi_HEP17       'efficiency vs jet #eta-#phi; jet #eta; jet #phi' jetEtaPhi_HEP17_numerator       jetEtaPhi_HEP17_denominator",
        "effic_jetMulti       'efficiency vs jet multiplicity; jet multiplicity; efficiency' jetMulti_numerator       jetMulti_denominator",
        "effic_muMulti       'efficiency vs muon multiplicity; muon multiplicity; efficiency' muMulti_numerator       muMulti_denominator",
        "effic_eleMulti       'efficiency vs electron multiplicity; electron multiplicity; efficiency' eleMulti_numerator       eleMulti_denominator",
        "effic_muPtEta_1       'efficiency vs muon pt-#eta; muon pt [GeV]; muon #eta' muPtEta_1_numerator       muPtEta_1_denominator",
        "effic_muEtaPhi_1       'efficiency vs muon #eta-#phi; muon #eta ; muon #phi' muEtaPhi_1_numerator       muEtaPhi_1_denominator",
        "effic_jetPtEta_1       'efficiency vs jet pt-#eta; jet pt [GeV]; jet #eta' jetPtEta_1_numerator       jetPtEta_1_denominator",
        "effic_jetEtaPhi_1       'efficiency vs jet #eta-#phi; jet #eta ; jet #phi' jetEtaPhi_1_numerator       jetEtaPhi_1_denominator",
        "effic_jetPtEta_2       'efficiency vs jet pt-#eta; jet pt [GeV]; jet #eta' jetPtEta_2_numerator       jetPtEta_2_denominator",
        "effic_jetEtaPhi_2       'efficiency vs jet #eta-#phi; jet #eta ; jet #phi' jetEtaPhi_2_numerator       jetEtaPhi_2_denominator",
        "effic_muPt_jetPt       'efficiency vs muon pt - jet pt; muon pt [GeV] ; jet pt [GeV]' muPt_jetPt_numerator       muPt_jetPt_denominator",
        "effic_muPt_eventHT       'efficiency vs muon pt - event HT; muon pt [GeV] ; event HT [GeV]' muPt_eventHT_numerator       muPt_eventHT_denominator",
        "effic_bjetPt_1      'efficiency vs 1st b-jet pt; bjet pt [GeV]; efficiency' bjetPt_1_numerator  bjetPt_1_denominator",
        "effic_bjetEta_1     'efficiency vs 1st b-jet eta; bjet eta ; efficiency'  bjetEta_1_numerator   bjetEta_1_denominator",
        "effic_bjetPhi_1     'efficiency vs 1st b-jet phi; bjet phi ; efficiency'  bjetPhi_1_numerator   bjetPhi_1_denominator",
        "effic_bjetDeepFlav_1     'efficiency vs 1st b-jet DeepJet Score; bjet DeepJet Score; efficiency' bjetDeepFlav_1_numerator  bjetDeepFlav_1_denominator",
        "effic_bjetPNet_1     'efficiency vs 1st b-jet PNet Score; bjet PNet Score; efficiency' bjetPNet_1_numerator  bjetPNet_1_denominator",
        "effic_bjetUParT_1     'efficiency vs 1st b-jet UParT Score; bjet UParT Score; efficiency' bjetUParT_1_numerator  bjetUParT_1_denominator",
        "effic_bjetMulti      'efficiency vs b-jet multiplicity; bjet multiplicity; efficiency' bjetMulti_numerator   bjetMulti_denominator",
        "effic_bjetPtEta_1    'efficiency vs 1st b-jet pt-#eta; jet pt [GeV]; bjet #eta' bjetPtEta_1_numerator   bjetPtEta_1_denominator",
        "effic_bjetEtaPhi_1    'efficiency vs 1st b-jet #eta-#phi; bjet #eta ; bjet #phi' bjetEtaPhi_1_numerator  bjetEtaPhi_1_denominator",
        "effic_DeltaR_jet_Mu    'efficiency vs #DeltaR between jet and mu; #DeltaR(jet,mu) ; efficiency' DeltaR_jet_Mu_numerator  DeltaR_jet_Mu_denominator",
    ),
)


BTVEfficiency_BTagMu_Jet = DQMEDHarvester("DQMGenericClient",
    subDirs        = cms.untracked.vstring("HLT/BTV/BTagMu_Jet/*"),
    verbose        = cms.untracked.uint32(0),
    resolution     = cms.vstring(),
    efficiency     = cms.vstring(
        "effic_met       'efficiency vs MET;MET [GeV];efficiency' met_numerator       met_denominator",
        "effic_metPhi       'efficiency vs MET #phi;MET #phi;efficiency' metPhi_numerator       metPhi_denominator",
        "effic_muPt_1       'efficiency vs muon pt; muon pt [GeV]; efficiency' muPt_1_numerator       muPt_1_denominator",
        "effic_muEta_1       'efficiency vs muon eta; muon eta ; efficiency' muEta_1_numerator       muEta_1_denominator",
        "effic_muPhi_1       'efficiency vs muon phi; muon phi ; efficiency' muPhi_1_numerator       muPhi_1_denominator",
        "effic_jetPt_1       'efficiency vs leading jet pt; jet pt [GeV]; efficiency' jetPt_1_numerator       jetPt_1_denominator",
        "effic_jetEta_1       'efficiency vs leading jet eta; jet eta ; efficiency' jetEta_1_numerator       jetEta_1_denominator",
        "effic_jetPhi_1       'efficiency vs leading jet phi; jet phi ; efficiency' jetPhi_1_numerator       jetPhi_1_denominator",
        "effic_eventHT       'efficiency vs event HT; event HT [GeV]; efficiency' effic_muPt_1_variableBinning       'efficiency vs muon pt; muon pt [GeV]; efficiency' muPt_1_variableBinning_numerator       muPt_1_variableBinning_denominator",
        "effic_jetEtaPhi_HEP17       'efficiency vs jet #eta-#phi; jet #eta; jet #phi' jetEtaPhi_HEP17_numerator       jetEtaPhi_HEP17_denominator",
        "effic_jetMulti       'efficiency vs jet multiplicity; jet multiplicity; efficiency' jetMulti_numerator       jetMulti_denominator",
        "effic_muMulti       'efficiency vs muon multiplicity; muon multiplicity; efficiency' muMulti_numerator       muMulti_denominator",
        "effic_eleMulti       'efficiency vs electron multiplicity; electron multiplicity; efficiency' eleMulti_numerator       eleMulti_denominator",
        "effic_muPtEta_1       'efficiency vs muon pt-#eta; muon pt [GeV]; muon #eta' muPtEta_1_numerator       muPtEta_1_denominator",
        "effic_muEtaPhi_1       'efficiency vs muon #eta-#phi; muon #eta ; muon #phi' muEtaPhi_1_numerator       muEtaPhi_1_denominator",
        "effic_jetPtEta_1       'efficiency vs jet pt-#eta; jet pt [GeV]; jet #eta' jetPtEta_1_numerator       jetPtEta_1_denominator",
        "effic_jetEtaPhi_1       'efficiency vs jet #eta-#phi; jet #eta ; jet #phi' jetEtaPhi_1_numerator       jetEtaPhi_1_denominator",
        "effic_muPt_jetPt       'efficiency vs muon pt - jet pt; muon pt [GeV] ; jet pt [GeV]' muPt_jetPt_numerator       muPt_jetPt_denominator",
        "effic_muPt_eventHT       'efficiency vs muon pt - event HT; muon pt [GeV] ; event HT [GeV]' muPt_eventHT_numerator       muPt_eventHT_denominator",
        "effic_bjetPt_1      'efficiency vs 1st b-jet pt; bjet pt [GeV]; efficiency' bjetPt_1_numerator  bjetPt_1_denominator",
        "effic_bjetEta_1     'efficiency vs 1st b-jet eta; bjet eta ; efficiency'  bjetEta_1_numerator   bjetEta_1_denominator",
        "effic_bjetPhi_1     'efficiency vs 1st b-jet phi; bjet phi ; efficiency'  bjetPhi_1_numerator   bjetPhi_1_denominator",
        "effic_bjetDeepFlav_1     'efficiency vs 1st b-jet DeepJet Score; bjet DeepJet Score; efficiency' bjetDeepFlav_1_numerator  bjetDeepFlav_1_denominator",
        "effic_bjetPNet_1     'efficiency vs 1st b-jet PNet Score; bjet PNet Score; efficiency' bjetPNet_1_numerator  bjetPNet_1_denominator",
        "effic_bjetUParT_1     'efficiency vs 1st b-jet UParT Score; bjet UParT Score; efficiency' bjetUParT_1_numerator  bjetUParT_1_denominator",
        "effic_bjetMulti      'efficiency vs b-jet multiplicity; bjet multiplicity; efficiency' bjetMulti_numerator   bjetMulti_denominator",
        "effic_bjetPtEta_1    'efficiency vs 1st b-jet pt-#eta; jet pt [GeV]; bjet #eta' bjetPtEta_1_numerator   bjetPtEta_1_denominator",
        "effic_bjetEtaPhi_1    'efficiency vs 1st b-jet #eta-#phi; bjet #eta ; bjet #phi' bjetEtaPhi_1_numerator  bjetEtaPhi_1_denominator",
        "effic_DeltaR_jet_Mu    'efficiency vs #DeltaR between jet and mu; #DeltaR(jet,mu) ; efficiency' DeltaR_jet_Mu_numerator  DeltaR_jet_Mu_denominator",
    ),
)


BTVEfficiency_BTagDiMu_Jet = DQMEDHarvester("DQMGenericClient",
    subDirs        = cms.untracked.vstring("HLT/BTV/BTagDiMu_Jet/*"),
    verbose        = cms.untracked.uint32(0),
    resolution     = cms.vstring(),
    efficiency     = cms.vstring(
        "effic_met       'efficiency vs MET;MET [GeV];efficiency' met_numerator       met_denominator",
        "effic_metPhi       'efficiency vs MET #phi;MET #phi;efficiency' metPhi_numerator       metPhi_denominator",
        "effic_muPt_1       'efficiency vs muon pt; muon pt [GeV]; efficiency' muPt_1_numerator       muPt_1_denominator",
        "effic_muEta_1       'efficiency vs muon eta; muon eta ; efficiency' muEta_1_numerator       muEta_1_denominator",
        "effic_muPhi_1       'efficiency vs muon phi; muon phi ; efficiency' muPhi_1_numerator       muPhi_1_denominator",
        "effic_muPt_2       'efficiency vs muon pt; muon pt [GeV]; efficiency' muPt_2_numerator       muPt_2_denominator",
        "effic_muEta_2       'efficiency vs muon eta; muon eta ; efficiency' muEta_2_numerator       muEta_2_denominator",
        "effic_muPhi_2       'efficiency vs muon phi; muon phi ; efficiency' muPhi_2_numerator       muPhi_2_denominator",
        "effic_jetPt_1       'efficiency vs leading jet pt; jet pt [GeV]; efficiency' jetPt_1_numerator       jetPt_1_denominator",
        "effic_jetEta_1       'efficiency vs leading jet eta; jet eta ; efficiency' jetEta_1_numerator       jetEta_1_denominator",
        "effic_jetPhi_1       'efficiency vs leading jet phi; jet phi ; efficiency' jetPhi_1_numerator       jetPhi_1_denominator",
        "effic_eventHT       'efficiency vs event HT; event HT [GeV]; efficiency' eventHT_numerator       eventHT_denominator",
        "effic_jetEtaPhi_HEP17       'efficiency vs jet #eta-#phi; jet #eta; jet #phi' jetEtaPhi_HEP17_numerator       jetEtaPhi_HEP17_denominator",
        "effic_jetMulti       'efficiency vs jet multiplicity; jet multiplicity; efficiency' jetMulti_numerator       jetMulti_denominator",
        "effic_muMulti       'efficiency vs muon multiplicity; muon multiplicity; efficiency' muMulti_numerator       muMulti_denominator",
        "effic_eleMulti       'efficiency vs electron multiplicity; electron multiplicity; efficiency' eleMulti_numerator       eleMulti_denominator",
        "effic_muPtEta_1       'efficiency vs muon pt-#eta; muon pt [GeV]; muon #eta' muPtEta_1_numerator       muPtEta_1_denominator",
        "effic_muEtaPhi_1       'efficiency vs muon #eta-#phi; muon #eta ; muon #phi' muEtaPhi_1_numerator       muEtaPhi_1_denominator",
        "effic_muPtEta_2       'efficiency vs muon pt-#eta; muon pt [GeV]; muon #eta' muPtEta_2_numerator       muPtEta_2_denominator",
        "effic_muEtaPhi_2       'efficiency vs muon #eta-#phi; muon #eta ; muon #phi' muEtaPhi_2_numerator       muEtaPhi_2_denominator",
        "effic_jetPtEta_1       'efficiency vs jet pt-#eta; jet pt [GeV]; jet #eta' jetPtEta_1_numerator       jetPtEta_1_denominator",
        "effic_jetEtaPhi_1       'efficiency vs jet #eta-#phi; jet #eta ; jet #phi' jetEtaPhi_1_numerator       jetEtaPhi_1_denominator",
        "effic_muPt_jetPt       'efficiency vs muon pt - jet pt; muon pt [GeV] ; jet pt [GeV]' muPt_jetPt_numerator       muPt_jetPt_denominator",
        "effic_muPt_eventHT       'efficiency vs muon pt - event HT; muon pt [GeV] ; event HT [GeV]' muPt_eventHT_numerator       muPt_eventHT_denominator",
        "effic_bjetPt_1      'efficiency vs 1st b-jet pt; bjet pt [GeV]; efficiency' bjetPt_1_numerator  bjetPt_1_denominator",
        "effic_bjetEta_1     'efficiency vs 1st b-jet eta; bjet eta ; efficiency'  bjetEta_1_numerator   bjetEta_1_denominator",
        "effic_bjetPhi_1     'efficiency vs 1st b-jet phi; bjet phi ; efficiency'  bjetPhi_1_numerator   bjetPhi_1_denominator",
        "effic_bjetDeepFlav_1     'efficiency vs 1st b-jet DeepJet Score; bjet DeepJet Score; efficiency' bjetDeepFlav_1_numerator  bjetDeepFlav_1_denominator",
        "effic_bjetPNet_1     'efficiency vs 1st b-jet PNet Score; bjet PNet Score; efficiency' bjetPNet_1_numerator  bjetPNet_1_denominator",
        "effic_bjetUParT_1     'efficiency vs 1st b-jet UParT Score; bjet UParT Score; efficiency' bjetUParT_1_numerator  bjetUParT_1_denominator",
        "effic_bjetMulti      'efficiency vs b-jet multiplicity; bjet multiplicity; efficiency' bjetMulti_numerator   bjetMulti_denominator",
        "effic_bjetPtEta_1    'efficiency vs 1st b-jet pt-#eta; jet pt [GeV]; bjet #eta' bjetPtEta_1_numerator   bjetPtEta_1_denominator",
        "effic_bjetEtaPhi_1    'efficiency vs 1st b-jet #eta-#phi; bjet #eta ; bjet #phi' bjetEtaPhi_1_numerator  bjetEtaPhi_1_denominator",
        "effic_DeltaR_jet_Mu    'efficiency vs #DeltaR between jet and mu; #DeltaR(jet,mu) ; efficiency' DeltaR_jet_Mu_numerator  DeltaR_jet_Mu_denominator",
    ),
)

BTVEfficiency_PFJet = DQMEDHarvester("DQMGenericClient",
    subDirs        = cms.untracked.vstring("HLT/BTV/PFJet/*"),
    verbose        = cms.untracked.uint32(0),
    resolution     = cms.vstring(),
    efficiency     = cms.vstring(
        "effic_met       'efficiency vs MET;MET [GeV];efficiency' met_numerator       met_denominator",
        "effic_metPhi       'efficiency vs MET #phi;MET #phi;efficiency' metPhi_numerator       metPhi_denominator",
        "effic_jetPt_1       'efficiency vs leading jet pt; jet pt [GeV]; efficiency' jetPt_1_numerator       jetPt_1_denominator",
        "effic_jetEta_1       'efficiency vs leading jet eta; jet eta ; efficiency' jetEta_1_numerator       jetEta_1_denominator",
        "effic_jetPhi_1       'efficiency vs leading jet phi; jet phi ; efficiency' jetPhi_1_numerator       jetPhi_1_denominator",
        "effic_eventHT       'efficiency vs event HT; event HT [GeV]; efficiency' eventHT_numerator       eventHT_denominator",
        "effic_jetEtaPhi_HEP17       'efficiency vs jet #eta-#phi; jet #eta; jet #phi' jetEtaPhi_HEP17_numerator       jetEtaPhi_HEP17_denominator",
        "effic_jetMulti       'efficiency vs jet multiplicity; jet multiplicity; efficiency' jetMulti_numerator       jetMulti_denominator",
        "effic_muMulti       'efficiency vs muon multiplicity; muon multiplicity; efficiency' muMulti_numerator       muMulti_denominator",
        "effic_eleMulti       'efficiency vs electron multiplicity; electron multiplicity; efficiency' eleMulti_numerator       eleMulti_denominator",
        "effic_jetPtEta_1       'efficiency vs jet pt-#eta; jet pt [GeV]; jet #eta' jetPtEta_1_numerator       jetPtEta_1_denominator",
        "effic_jetEtaPhi_1       'efficiency vs jet #eta-#phi; jet #eta ; jet #phi' jetEtaPhi_1_numerator       jetEtaPhi_1_denominator",
        "effic_bjetPt_1      'efficiency vs 1st b-jet pt; bjet pt [GeV]; efficiency' bjetPt_1_numerator  bjetPt_1_denominator",
        "effic_bjetEta_1     'efficiency vs 1st b-jet eta; bjet eta ; efficiency'  bjetEta_1_numerator   bjetEta_1_denominator",
        "effic_bjetPhi_1     'efficiency vs 1st b-jet phi; bjet phi ; efficiency'  bjetPhi_1_numerator   bjetPhi_1_denominator",
        "effic_bjetDeepFlav_1     'efficiency vs 1st b-jet DeepJet Score; bjet DeepJet Score; efficiency' bjetDeepFlav_1_numerator  bjetDeepFlav_1_denominator",
        "effic_bjetPNet_1     'efficiency vs 1st b-jet PNet Score; bjet PNet Score; efficiency' bjetPNet_1_numerator  bjetPNet_1_denominator",
        "effic_bjetUParT_1     'efficiency vs 1st b-jet UParT Score; bjet UParT Score; efficiency' bjetUParT_1_numerator  bjetUParT_1_denominator",
        "effic_bjetMulti      'efficiency vs b-jet multiplicity; bjet multiplicity; efficiency' bjetMulti_numerator   bjetMulti_denominator",
        "effic_bjetPtEta_1    'efficiency vs 1st b-jet pt-#eta; jet pt [GeV]; bjet #eta' bjetPtEta_1_numerator   bjetPtEta_1_denominator",
        "effic_bjetEtaPhi_1    'efficiency vs 1st b-jet #eta-#phi; bjet #eta ; bjet #phi' bjetEtaPhi_1_numerator  bjetEtaPhi_1_denominator",
        "effic_DeltaR_jet_Mu    'efficiency vs #DeltaR between jet and mu; #DeltaR(jet,mu) ; efficiency' DeltaR_jet_Mu_numerator  DeltaR_jet_Mu_denominator",
    ),
)

BTVEfficiency_TurnOnCurves = DQMEDHarvester("DQMGenericClient",
    subDirs        = cms.untracked.vstring(
        "HLT/BTV/HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_PFDiJet30*",
    ),
    verbose        = cms.untracked.uint32(0),
    resolution     = cms.vstring(),
    efficiency     = cms.vstring(
        "Turnon_loose   'turn-on (loose online OP);discriminator;efficiency'   Turnon_loose_Discr_numerator   Turnon_loose_Discr_denominator",
        "Turnon_medium  'turn-on (medium online OP);discriminator;efficiency'  Turnon_medium_Discr_numerator  Turnon_medium_Discr_denominator",
        "Turnon_tight   'turn-on (tight online OP);discriminator;efficiency'   Turnon_tight_Discr_numerator   Turnon_tight_Discr_denominator",
        "Turnon_loose_pt    'turn-on (loose online OP);pt;efficiency'    Turnon_loose_Pt_numerator    Turnon_loose_Pt_denominator",
        "Turnon_medium_pt   'turn-on (medium online OP);pt;efficiency'   Turnon_medium_Pt_numerator   Turnon_medium_Pt_denominator",
        "Turnon_tight_pt    'turn-on (tight online OP);pt;efficiency'    Turnon_tight_Pt_numerator    Turnon_tight_Pt_denominator",
        "Turnon_loose_eta   'turn-on (loose online OP);eta;efficiency'   Turnon_loose_Eta_numerator   Turnon_loose_Eta_denominator",
        "Turnon_medium_eta  'turn-on (medium online OP);eta;efficiency'  Turnon_medium_Eta_numerator  Turnon_medium_Eta_denominator",
        "Turnon_tight_eta   'turn-on (tight online OP);eta;efficiency'   Turnon_tight_Eta_numerator   Turnon_tight_Eta_denominator",
        "Turnon_loose_phi   'turn-on (loose online OP);phi;efficiency'   Turnon_loose_Phi_numerator   Turnon_loose_Phi_denominator",
        "Turnon_medium_phi  'turn-on (medium online OP);phi;efficiency'  Turnon_medium_Phi_numerator  Turnon_medium_Phi_denominator",
        "Turnon_tight_phi   'turn-on (tight online OP);phi;efficiency'   Turnon_tight_Phi_numerator   Turnon_tight_Phi_denominator",
    ),
)

BTVEfficiency_OnlineTrackEff = DQMEDHarvester("DQMGenericClient",
    subDirs        = cms.untracked.vstring(
        "HLT/BTV/HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_PFDiJet30*",
    ),
    verbose        = cms.untracked.uint32(0),
    resolution     = cms.vstring(),
    efficiency     = cms.vstring(
        "OnlineTrkEff_Pt   'Relative Online Track Eff vs Pt;Pt;relative efficiency'   OnlineTrkEff_Pt_numerator   OnlineTrkEff_Pt_denominator",
        "OnlineTrkEff_Eta  'Relative Online Track Eff vs Eta;Eta;relative efficiency' OnlineTrkEff_Eta_numerator   OnlineTrkEff_Eta_denominator",
        "OnlineTrkEff_3d_ip_distance  'Relative Online Track Eff vs IP3D;IP3D;relative efficiency' OnlineTrkEff_3d_ip_distance_numerator   OnlineTrkEff_3d_ip_distance_denominator",
        "OnlineTrkEff_3d_ip_sig  'Relative Online Track Eff vs IP3D signifance;IP3D significance;relative efficiency' OnlineTrkEff_3d_ip_sig_numerator   OnlineTrkEff_3d_ip_sig_denominator",
    ),
)

BTVEfficiency_OnlineTrackFake = DQMEDHarvester("DQMGenericClient",
    subDirs        = cms.untracked.vstring(
        "HLT/BTV/HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_PFDiJet30*",
    ),
    verbose        = cms.untracked.uint32(0),
    resolution     = cms.vstring(),
    efficiency     = cms.vstring(
        "OnlineTrkFake_Pt   'Relative Online Fake Rate vs Pt;Pt;relative fake rate'   OnlineTrkFake_Pt_numerator   OnlineTrkFake_Pt_denominator",
        "OnlineTrkFake_Eta  'Relative Online Fake Rate vs Eta;Eta;relative fake rate' OnlineTrkFake_Eta_numerator   OnlineTrkFake_Eta_denominator",
        "OnlineTrkFake_3d_ip_distance  'Relative Online Fake Rate vs IP3D;IP3D;relative fake rate' OnlineTrkFake_3d_ip_distance_numerator   OnlineTrkFake_3d_ip_distance_denominator",
        "OnlineTrkFake_3d_ip_sig  'Relative Online Fake Rate vs IP3D signifance;IP3D significance;relative fake rate' OnlineTrkFake_3d_ip_sig_numerator   OnlineTrkFake_3d_ip_sig_denominator",
    ),
)

from DQMOffline.Trigger.TrackingMonitoring_Client_cff import TrackToTrackEfficiencies

BJetTrackToTrackEfficiencies = TrackToTrackEfficiencies.clone(
    subDirs = ["HLT/BTV/HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_PFDiJet30*",
              ]
)

btaggingClient = cms.Sequence(
    BTVEfficiency_TurnOnCurves
  + BTVEfficiency_OnlineTrackEff
  + BTVEfficiency_OnlineTrackFake
  + BTVEfficiency_BTagMu_DiJet
  + BTVEfficiency_BTagMu_Jet
  + BTVEfficiency_BTagDiMu_Jet
  + BTVEfficiency_PFJet
  + BJetTrackToTrackEfficiencies
)
