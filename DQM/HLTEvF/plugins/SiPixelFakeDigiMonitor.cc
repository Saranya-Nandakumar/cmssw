// -*- C++ -*-
//
// Package:    DQM/HLTEvF
// Class:      SiPixelFakeDigiMonitor
//
// DQM Monitoring for fake/recovered digis and duplicate pixels from pixel clustering.
// Monitors pixels that are filled in during digi morphing to maintain cluster contiguity.
//

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/SiPixelClusterSoA/interface/SiPixelClustersHost.h"
#include "DataFormats/SiPixelDigiSoA/interface/SiPixelDigisHost.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/TrackerCommon/interface/PixelBarrelName.h"
#include "DataFormats/TrackerCommon/interface/PixelEndcapName.h"

#include "Geometry/Records/interface/TrackerTopologyRcd.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include <map>
#include <set>
#include <atomic>
#include <unordered_map>

class SiPixelFakeDigiMonitor : public DQMEDAnalyzer {
public:
  explicit SiPixelFakeDigiMonitor(const edm::ParameterSet&);
  ~SiPixelFakeDigiMonitor() override = default;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

protected:
  void bookHistograms(DQMStore::IBooker&, edm::Run const&, edm::EventSetup const&) override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;

private:
  // Parameters
  const edm::InputTag clustersSoASrc_;
  const edm::InputTag digisSoASrc_;
  const edm::InputTag digiSrc_;
  const std::string topFolderName_;

  // Tokens
  const edm::EDGetTokenT<SiPixelClustersHost> clustersSoAToken_;
  const edm::EDGetTokenT<SiPixelDigisHost> digisSoAToken_;
  const edm::EDGetTokenT<edm::DetSetVector<PixelDigi>> digiToken_;
  const edm::ESGetToken<TrackerTopology, TrackerTopologyRcd> trackerTopoToken_;
  const edm::ESGetToken<TrackerTopology, TrackerTopologyRcd> trackerTopoTokenBeginRun_;
  const edm::ESGetToken<TrackerGeometry, TrackerDigiGeometryRecord> trackerGeomTokenBeginRun_;

  // Histograms - Fake Digis
  MonitorElement* meFakeDigisPerEvent_;
  MonitorElement* meFakeDigisPerModule_;
  MonitorElement* meFakeDigisPerModuleBarrel_;
  MonitorElement* meModulesWithFakeDigis_;
  MonitorElement* meNFakeDigisBarrel_;
  std::vector<MonitorElement*> meNFakeDigisLayer_;
  std::vector<MonitorElement*> meNFakeDigisDisk_;
  std::vector<MonitorElement*> meFakeDigiOccupancyBarrelLayer_;

  // Fake Digis - HLT monitoring
  MonitorElement* meFakeDigisPerEventVsLS_;
  MonitorElement* meFakeDigisFractionVsLS_;

  // Histograms - Duplicate Pixels
  MonitorElement* meDuplicatePixelsPerEvent_;
  MonitorElement* meDuplicatePixelsPerModule_;
  MonitorElement* meDuplicatePixelsPerModuleBarrel_;
  MonitorElement* meDuplicatePixelsPerModuleEndcap_;
  MonitorElement* meModulesWithDuplicatePixels_;
  MonitorElement* meNDuplicatePixelsBarrel_;
  MonitorElement* meNDuplicatePixelsEndcap_;
  std::vector<MonitorElement*> meNDuplicatePixelsLayer_;
  std::vector<MonitorElement*> meNDuplicatePixelsDisk_;
  std::vector<MonitorElement*> meDuplicatePixelOccupancyBarrelLayer_;

  // Duplicate Pixels - HLT monitoring
  MonitorElement* meDuplicatePixelsPerEventVsLS_;
  MonitorElement* meDuplicatePixelsFractionVsLS_;

  // Geometry information
  unsigned int nBarrelLayers_;
  unsigned int nEndcapDisks_;
  std::vector<unsigned int> nLaddersPerLayer_;
  static constexpr int nModulesPerLadder_ = 8;

  // Lumisection counters
  std::atomic<unsigned long long> fakeDigisThisLS_;
  std::atomic<unsigned long long> duplicatePixelsThisLS_;
  std::atomic<unsigned long long> totalDigisThisLS_;
  std::atomic<unsigned long long> eventsThisLS_;
};

SiPixelFakeDigiMonitor::SiPixelFakeDigiMonitor(const edm::ParameterSet& iConfig)
    : clustersSoASrc_(iConfig.getParameter<edm::InputTag>("clustersSoASrc")),
      digisSoASrc_(iConfig.getParameter<edm::InputTag>("digisSoASrc")),
      digiSrc_(iConfig.getParameter<edm::InputTag>("digiSrc")),
      topFolderName_(iConfig.getParameter<std::string>("topFolderName")),
      clustersSoAToken_(consumes<SiPixelClustersHost>(clustersSoASrc_)),
      digisSoAToken_(consumes<SiPixelDigisHost>(digisSoASrc_)),
      digiToken_(consumes<edm::DetSetVector<PixelDigi>>(digiSrc_)),
      trackerTopoToken_(esConsumes<TrackerTopology, TrackerTopologyRcd>()),
      trackerTopoTokenBeginRun_(esConsumes<TrackerTopology, TrackerTopologyRcd, edm::Transition::BeginRun>()),
      trackerGeomTokenBeginRun_(esConsumes<TrackerGeometry, TrackerDigiGeometryRecord, edm::Transition::BeginRun>()),
      fakeDigisThisLS_(0),
      duplicatePixelsThisLS_(0),
      totalDigisThisLS_(0),
      eventsThisLS_(0) {
  edm::LogInfo("SiPixelFakeDigiMonitor") << "Monitoring fake digis from clusters SoA: " << clustersSoASrc_;
}

void SiPixelFakeDigiMonitor::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("clustersSoASrc", edm::InputTag("siPixelClustersPreSplittingAlpaka"));
  desc.add<edm::InputTag>("digisSoASrc", edm::InputTag("siPixelDigisSoAAlpaka"));
  desc.add<edm::InputTag>("digiSrc", edm::InputTag("siPixelDigis"));
  desc.add<std::string>("topFolderName", "Pixel");
  descriptions.addDefault(desc);
}

void SiPixelFakeDigiMonitor::bookHistograms(DQMStore::IBooker& iBooker,
                                            edm::Run const& iRun,
                                            edm::EventSetup const& iSetup) {
  const TrackerTopology& tTopo = iSetup.getData(trackerTopoTokenBeginRun_);
  const TrackerGeometry& trackerGeom = iSetup.getData(trackerGeomTokenBeginRun_);

  // Determine geometry from TrackerGeometry
  nBarrelLayers_ = 0;
  nEndcapDisks_ = 0;
  std::map<unsigned int, std::set<unsigned int>> laddersPerLayer;

  for (const auto& det : trackerGeom.detUnits()) {
    DetId detId = det->geographicalId();
    if (detId.det() != DetId::Tracker)
      continue;

    if (detId.subdetId() == PixelSubdetector::PixelBarrel) {
      unsigned int layer = tTopo.pxbLayer(detId);
      unsigned int ladder = tTopo.pxbLadder(detId);
      if (layer > nBarrelLayers_)
        nBarrelLayers_ = layer;
      laddersPerLayer[layer].insert(ladder);
    } else if (detId.subdetId() == PixelSubdetector::PixelEndcap) {
      unsigned int disk = tTopo.pxfDisk(detId);
      if (disk > nEndcapDisks_)
        nEndcapDisks_ = disk;
    }
  }

  nLaddersPerLayer_.resize(nBarrelLayers_, 0);
  for (unsigned int layer = 1; layer <= nBarrelLayers_; ++layer) {
    nLaddersPerLayer_[layer - 1] = laddersPerLayer[layer].size();
  }

  // Resize histogram vectors
  meNFakeDigisLayer_.resize(nBarrelLayers_, nullptr);
  meNFakeDigisDisk_.resize(nEndcapDisks_, nullptr);
  meFakeDigiOccupancyBarrelLayer_.resize(nBarrelLayers_, nullptr);
  meNDuplicatePixelsLayer_.resize(nBarrelLayers_, nullptr);
  meNDuplicatePixelsDisk_.resize(nEndcapDisks_, nullptr);
  meDuplicatePixelOccupancyBarrelLayer_.resize(nBarrelLayers_, nullptr);

  edm::LogInfo("SiPixelFakeDigiMonitor") << "Geometry: " << nBarrelLayers_ << " barrel layers, "
                                         << nEndcapDisks_ << " endcap disks";

  // ============ Fake Digis Histograms ============
  iBooker.setCurrentFolder(topFolderName_ + "/FakeDigis");

  meFakeDigisPerEvent_ = iBooker.book1D(
      "FakeDigisPerEvent", "Number of Fake Digis per Event;Fake Digis;Events", 500, 0., 8000.);
  meFakeDigisPerModule_ = iBooker.book1D(
      "FakeDigisPerModule", "Number of Fake Digis per Module;Fake Digis;Modules", 2000, 0., 8000.);
  meFakeDigisPerModuleBarrel_ = iBooker.book1D(
      "FakeDigisPerModuleBarrel", "Fake Digis per Module (Barrel);Fake Digis;Modules", 100, 0., 8000.);
  meModulesWithFakeDigis_ = iBooker.book1D(
      "ModulesWithFakeDigis", "Number of Modules with Fake Digis;Modules;Events", 100, 0., 2000.);
  meNFakeDigisBarrel_ = iBooker.book1D(
      "NFakeDigisBarrel", "Number of Fake Digis (Barrel);Fake Digis;Events", 500, 0., 8000.);

  iBooker.setCurrentFolder(topFolderName_ + "/FakeDigis/Barrel");
  for (unsigned int i = 0; i < nBarrelLayers_; i++) {
    std::string hname = "NFakeDigisLayer" + std::to_string(i + 1);
    std::string htitle = "Number of Fake Digis (Layer " + std::to_string(i + 1) + ");Fake Digis;Events";
    meNFakeDigisLayer_[i] = iBooker.book1D(hname, htitle, 200, 0., 8000.);
  }

  iBooker.setCurrentFolder(topFolderName_ + "/FakeDigis/Endcap");
  for (unsigned int i = 0; i < nEndcapDisks_; i++) {
    std::string hname = "NFakeDigisDisk" + std::to_string(i + 1);
    std::string htitle = "Number of Fake Digis (Disk " + std::to_string(i + 1) + " +/-);Fake Digis;Events";
    meNFakeDigisDisk_[i] = iBooker.book1D(hname, htitle, 200, 0., 8000.);
  }

  iBooker.setCurrentFolder(topFolderName_ + "/FakeDigis/Occupancy");
  for (unsigned int i = 0; i < nBarrelLayers_; i++) {
    int nLadders = nLaddersPerLayer_[i];
    int halfLadders = nLadders / 2;
    std::string hname = "FakeDigiOccupancyBarrelLayer" + std::to_string(i + 1);
    std::string htitle = "Fake Digi Occupancy (Barrel Layer " + std::to_string(i + 1) + ");Module;Ladder";
    meFakeDigiOccupancyBarrelLayer_[i] = iBooker.book2D(
        hname, htitle, nModulesPerLadder_ + 1, -4.5, 4.5, nLadders + 1, -(halfLadders + 0.5), halfLadders + 0.5);
    meFakeDigiOccupancyBarrelLayer_[i]->setOption("colz");
    meFakeDigiOccupancyBarrelLayer_[i]->getTH2F()->SetMinimum(0.001);
    for (int j = 1; j <= nModulesPerLadder_ + 1; j++) {
      meFakeDigiOccupancyBarrelLayer_[i]->setBinLabel(j, std::to_string(j - 5), 1);
    }
  }

  iBooker.setCurrentFolder(topFolderName_ + "/FakeDigis/HLT");
  meFakeDigisPerEventVsLS_ = iBooker.bookProfile(
      "FakeDigisPerEventVsLS", "Fake Digis per Event vs LS;Lumisection;Fake Digis/Event", 2000, 0., 2000., 0., 8000.);
  meFakeDigisFractionVsLS_ = iBooker.bookProfile(
      "FakeDigisFractionVsLS", "Fake Digis Fraction vs LS;Lumisection;Fake Fraction", 2000, 0., 2000., 0., 1.0);

  // ============ Duplicate Pixels Histograms ============
  iBooker.setCurrentFolder(topFolderName_ + "/DuplicatePixels");

  meDuplicatePixelsPerEvent_ = iBooker.book1D(
      "DuplicatePixelsPerEvent", "Number of Duplicate Pixels per Event;Duplicate Pixels;Events", 500, 0., 500.);
  meDuplicatePixelsPerModule_ = iBooker.book1D(
      "DuplicatePixelsPerModule", "Number of Duplicate Pixels per Module;Duplicate Pixels;Modules", 200, 0., 500.);
  meDuplicatePixelsPerModuleBarrel_ = iBooker.book1D(
      "DuplicatePixelsPerModuleBarrel", "Duplicate Pixels per Module (Barrel);Duplicate Pixels;Modules", 200, 0., 500.);
  meDuplicatePixelsPerModuleEndcap_ = iBooker.book1D(
      "DuplicatePixelsPerModuleEndcap", "Duplicate Pixels per Module (Endcap);Duplicate Pixels;Modules", 200, 0., 500.);
  meModulesWithDuplicatePixels_ = iBooker.book1D(
      "ModulesWithDuplicatePixels", "Number of Modules with Duplicate Pixels;Modules;Events", 100, 0., 100.);
  meNDuplicatePixelsBarrel_ = iBooker.book1D(
      "NDuplicatePixelsBarrel", "Number of Duplicate Pixels (Barrel);Duplicate Pixels;Events", 500, 0., 500.);
  meNDuplicatePixelsEndcap_ = iBooker.book1D(
      "NDuplicatePixelsEndcap", "Number of Duplicate Pixels (Endcap);Duplicate Pixels;Events", 500, 0., 500.);

  iBooker.setCurrentFolder(topFolderName_ + "/DuplicatePixels/Barrel");
  for (unsigned int i = 0; i < nBarrelLayers_; i++) {
    meNDuplicatePixelsLayer_[i] = iBooker.book1D(
        "NDuplicatePixelsLayer" + std::to_string(i + 1),
        "Number of Duplicate Pixels (Layer " + std::to_string(i + 1) + ");Duplicate Pixels;Events", 200, 0., 500.);
  }

  iBooker.setCurrentFolder(topFolderName_ + "/DuplicatePixels/Endcap");
  for (unsigned int i = 0; i < nEndcapDisks_; i++) {
    meNDuplicatePixelsDisk_[i] = iBooker.book1D(
        "NDuplicatePixelsDisk" + std::to_string(i + 1),
        "Number of Duplicate Pixels (Disk " + std::to_string(i + 1) + " +/-);Duplicate Pixels;Events", 200, 0., 500.);
  }

  iBooker.setCurrentFolder(topFolderName_ + "/DuplicatePixels/Occupancy");
  for (unsigned int i = 0; i < nBarrelLayers_; i++) {
    int nLadders = nLaddersPerLayer_[i];
    int halfLadders = nLadders / 2;
    std::string hname = "DuplicatePixelOccupancyBarrelLayer" + std::to_string(i + 1);
    std::string htitle = "Duplicate Pixel Occupancy (Barrel Layer " + std::to_string(i + 1) + ");Module;Ladder";
    meDuplicatePixelOccupancyBarrelLayer_[i] = iBooker.book2D(
        hname, htitle, nModulesPerLadder_ + 1, -4.5, 4.5, nLadders + 1, -(halfLadders + 0.5), halfLadders + 0.5);
    meDuplicatePixelOccupancyBarrelLayer_[i]->setOption("colz");
    meDuplicatePixelOccupancyBarrelLayer_[i]->getTH2F()->SetMinimum(0.001);
    for (int j = 1; j <= nModulesPerLadder_ + 1; j++) {
      meDuplicatePixelOccupancyBarrelLayer_[i]->setBinLabel(j, std::to_string(j - 5), 1);
    }
  }

  iBooker.setCurrentFolder(topFolderName_ + "/DuplicatePixels/HLT");
  meDuplicatePixelsPerEventVsLS_ = iBooker.bookProfile(
      "DuplicatePixelsPerEventVsLS", "Duplicate Pixels per Event vs LS;Lumisection;Duplicate Pixels/Event",
      2000, 0., 2000., 0., 500.);
  meDuplicatePixelsFractionVsLS_ = iBooker.bookProfile(
      "DuplicatePixelsFractionVsLS", "Duplicate Pixels Fraction vs LS;Lumisection;Duplicate Fraction",
      2000, 0., 2000., 0., 1.0);

  edm::LogInfo("SiPixelFakeDigiMonitor") << "Histograms booked with " << nBarrelLayers_ << " barrel layers and "
                                         << nEndcapDisks_ << " endcap disks from geometry.";
}

void SiPixelFakeDigiMonitor::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  const TrackerTopology& tTopo = iSetup.getData(trackerTopoToken_);

  edm::Handle<SiPixelClustersHost> clustersSoA;
  iEvent.getByToken(clustersSoAToken_, clustersSoA);

  if (!clustersSoA.isValid())
    return;

  edm::Handle<edm::DetSetVector<PixelDigi>> digis;
  iEvent.getByToken(digiToken_, digis);

  int totalDigis = 0;
  if (digis.isValid()) {
    for (const auto& detSet : *digis)
      totalDigis += detSet.size();
  }

  auto const& clusters_view = clustersSoA->const_view();
  const uint32_t nModules = clusters_view[0].moduleStart();

  int totalFakeDigis = 0;
  int fakeDigisBarrel = 0;
  std::vector<int> fakeDigisPerLayer(nBarrelLayers_, 0);
  std::vector<int> fakeDigisPerDisk(nEndcapDisks_, 0);
  int modulesWithFakeDigis = 0;

  int totalDuplicatePixels = 0;
  int duplicatePixelsBarrel = 0;
  int duplicatePixelsEndcap = 0;
  std::vector<int> duplicatePixelsPerLayer(nBarrelLayers_, 0);
  std::vector<int> duplicatePixelsPerDisk(nEndcapDisks_, 0);
  int modulesWithDuplicatePixels = 0;

  for (uint32_t i = 1; i <= nModules; ++i) {
    uint32_t nFakeDigis = clusters_view[i].nFakeDigis();
    uint32_t nDuplicatePixels = clusters_view[i].nDuplicatePixels();

    if (nFakeDigis == 0 && nDuplicatePixels == 0)
      continue;

    uint32_t rawId = clusters_view[i].rawId();
    DetId detIdObj(rawId);
    if (detIdObj.det() != DetId::Tracker)
      continue;

    int subdet = detIdObj.subdetId();

    if (nFakeDigis > 0) {
      totalFakeDigis += nFakeDigis;
      modulesWithFakeDigis++;
      meFakeDigisPerModule_->Fill(nFakeDigis);
      if (subdet == static_cast<int>(PixelSubdetector::PixelBarrel)) {
        meFakeDigisPerModuleBarrel_->Fill(nFakeDigis);
        fakeDigisBarrel += nFakeDigis;
        unsigned int layer = tTopo.pxbLayer(detIdObj);
        if (layer >= 1 && layer <= nBarrelLayers_) {
          fakeDigisPerLayer[layer - 1] += nFakeDigis;
          unsigned int module = tTopo.pxbModule(detIdObj);
          unsigned int ladder = tTopo.pxbLadder(detIdObj);
          unsigned int halfLadders = nLaddersPerLayer_[layer - 1] / 2;
          int signedLadder = (ladder <= halfLadders)
                                 ? static_cast<int>(ladder) - static_cast<int>(halfLadders) - 1
                                 : static_cast<int>(ladder) - static_cast<int>(halfLadders);
          int signedModule = (module <= 4u) ? static_cast<int>(module) - 5 : static_cast<int>(module) - 4;
          meFakeDigiOccupancyBarrelLayer_[layer - 1]->Fill(signedModule, signedLadder);
        }
      } else if (subdet == static_cast<int>(PixelSubdetector::PixelEndcap)) {
        unsigned int disk = tTopo.pxfDisk(detIdObj);
        if (disk >= 1 && disk <= nEndcapDisks_)
          fakeDigisPerDisk[disk - 1] += nFakeDigis;
      }
    }

    if (nDuplicatePixels > 0) {
      totalDuplicatePixels += nDuplicatePixels;
      modulesWithDuplicatePixels++;
      meDuplicatePixelsPerModule_->Fill(nDuplicatePixels);
      if (subdet == static_cast<int>(PixelSubdetector::PixelBarrel)) {
        meDuplicatePixelsPerModuleBarrel_->Fill(nDuplicatePixels);
        duplicatePixelsBarrel += nDuplicatePixels;
        unsigned int layer = tTopo.pxbLayer(detIdObj);
        if (layer >= 1 && layer <= nBarrelLayers_) {
          duplicatePixelsPerLayer[layer - 1] += nDuplicatePixels;
          unsigned int module = tTopo.pxbModule(detIdObj);
          unsigned int ladder = tTopo.pxbLadder(detIdObj);
          unsigned int halfLadders = nLaddersPerLayer_[layer - 1] / 2;
          int signedLadder = (ladder <= halfLadders)
                                 ? static_cast<int>(ladder) - static_cast<int>(halfLadders) - 1
                                 : static_cast<int>(ladder) - static_cast<int>(halfLadders);
          int signedModule = (module <= 4u) ? static_cast<int>(module) - 5 : static_cast<int>(module) - 4;
          meDuplicatePixelOccupancyBarrelLayer_[layer - 1]->Fill(signedModule, signedLadder);
        }
      } else if (subdet == static_cast<int>(PixelSubdetector::PixelEndcap)) {
        meDuplicatePixelsPerModuleEndcap_->Fill(nDuplicatePixels);
        duplicatePixelsEndcap += nDuplicatePixels;
        unsigned int disk = tTopo.pxfDisk(detIdObj);
        if (disk >= 1 && disk <= nEndcapDisks_)
          duplicatePixelsPerDisk[disk - 1] += nDuplicatePixels;
      }
    }
  }

  // Event-level fills
  meFakeDigisPerEvent_->Fill(totalFakeDigis);
  meModulesWithFakeDigis_->Fill(modulesWithFakeDigis);
  meNFakeDigisBarrel_->Fill(fakeDigisBarrel);
  meDuplicatePixelsPerEvent_->Fill(totalDuplicatePixels);
  meModulesWithDuplicatePixels_->Fill(modulesWithDuplicatePixels);
  meNDuplicatePixelsBarrel_->Fill(duplicatePixelsBarrel);
  meNDuplicatePixelsEndcap_->Fill(duplicatePixelsEndcap);

  // LS Accumulation
  fakeDigisThisLS_ += totalFakeDigis;
  duplicatePixelsThisLS_ += totalDuplicatePixels;
  totalDigisThisLS_ += totalDigis;
  eventsThisLS_++;

  for (unsigned int i = 0; i < nBarrelLayers_; i++) {
    if (fakeDigisPerLayer[i] > 0)
      meNFakeDigisLayer_[i]->Fill(fakeDigisPerLayer[i]);
    if (duplicatePixelsPerLayer[i] > 0)
      meNDuplicatePixelsLayer_[i]->Fill(duplicatePixelsPerLayer[i]);
  }
  for (unsigned int i = 0; i < nEndcapDisks_; i++) {
    if (fakeDigisPerDisk[i] > 0)
      meNFakeDigisDisk_[i]->Fill(fakeDigisPerDisk[i]);
    if (duplicatePixelsPerDisk[i] > 0)
      meNDuplicatePixelsDisk_[i]->Fill(duplicatePixelsPerDisk[i]);
  }
}

DEFINE_FWK_MODULE(SiPixelFakeDigiMonitor);
