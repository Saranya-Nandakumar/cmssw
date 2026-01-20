#include "RecoLocalTracker/SiPixelClusterizer/plugins/SiPixelFakeDigiMonitor.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/TrackerCommon/interface/PixelBarrelName.h"
#include "DataFormats/TrackerCommon/interface/PixelEndcapName.h"

SiPixelFakeDigiMonitor::SiPixelFakeDigiMonitor(const edm::ParameterSet& iConfig)
    : clustersSoASrc_(iConfig.getParameter<edm::InputTag>("clustersSoASrc")),
      digisSoASrc_(iConfig.getParameter<edm::InputTag>("digisSoASrc")),
      digiSrc_(iConfig.getParameter<edm::InputTag>("digiSrc")),
      topFolderName_(iConfig.getParameter<std::string>("topFolderName")),
      clustersSoAToken_(consumes<SiPixelClustersHost>(clustersSoASrc_)),
      digisSoAToken_(consumes<SiPixelDigisHost>(digisSoASrc_)),
      digiToken_(consumes<edm::DetSetVector<PixelDigi>>(digiSrc_)),
      trackerTopoToken_(esConsumes<TrackerTopology, TrackerTopologyRcd>()),
      trackerGeomToken_(esConsumes<TrackerGeometry, TrackerDigiGeometryRecord>()),
      fakeDigisThisLS_(0),
      totalDigisThisLS_(0),
      eventsThisLS_(0) {
  edm::LogInfo("SiPixelFakeDigiMonitor") << "Monitoring fake digis from clusters SoA: " << clustersSoASrc_;
}

SiPixelFakeDigiMonitor::~SiPixelFakeDigiMonitor() {}

void SiPixelFakeDigiMonitor::bookHistograms(DQMStore::IBooker& iBooker,
                                            edm::Run const& iRun,
                                            edm::EventSetup const& iSetup) {
  iBooker.setCurrentFolder(topFolderName_ + "/FakeDigis");

  // Event-level histograms
  meFakeDigisPerEvent_ = iBooker.book1D("FakeDigisPerEvent",
                                      "Number of Fake Digis per Event;Fake Digis;Events",
                                      500,
                                      0.,
                                      5000.);

  // Module-level histograms
  meFakeDigisPerModule_ = iBooker.book1D("FakeDigisPerModule",
                                        "Number of Fake Digis per Module;Fake Digis;Modules",
                                        100,
                                        0.,
                                        100.);

  meFakeDigisPerModuleBarrel_ = iBooker.book1D("FakeDigisPerModuleBarrel",
                                               "Fake Digis per Module (Barrel);Fake Digis;Modules",
                                               100,
                                               0.,
                                               100.);

  meModulesWithFakeDigis_ = iBooker.book1D("ModulesWithFakeDigis",
                                           "Number of Modules with Fake Digis;Modules;Events",
                                           100,
                                           0.,
                                           100.);

  // Cluster-level histograms
  meFakeDigisPerCluster_ = iBooker.book1D("FakeDigisPerCluster",
                                         "Fake Digis per Cluster;Fake Digis;Clusters",
                                         50,
                                         0.,
                                         50.);

  // Barrel Summary
  meNFakeDigisBarrel_ = iBooker.book1D("NFakeDigisBarrel",
                                       "Number of Fake Digis (Barrel);Fake Digis;Events",
                                       500,
                                       0.,
                                       5000.);

  // Per-layer histograms
  iBooker.setCurrentFolder(topFolderName_ + "/FakeDigis/Barrel");
  for (int i = 0; i < 4; i++) {
    std::string hname = "NFakeDigisLayer" + std::to_string(i + 1);
    std::string htitle = "Number of Fake Digis (Layer " + std::to_string(i + 1) + ");Fake Digis;Events";
    meNFakeDigisLayer_[i] = iBooker.book1D(hname, htitle, 200, 0., 2000.);
  }

  // Per-disk histograms
  iBooker.setCurrentFolder(topFolderName_ + "/FakeDigis/Endcap");
  for (int i = 0; i < 3; i++) {
    std::string hname = "NFakeDigisDisk" + std::to_string(i + 1);
    std::string htitle = "Number of Fake Digis (Disk " + std::to_string(i + 1) + " +/-);Fake Digis;Events";
    meNFakeDigisDisk_[i] = iBooker.book1D(hname, htitle, 200, 0., 2000.);
  }

  // Occupancy maps
  iBooker.setCurrentFolder(topFolderName_ + "/FakeDigis/Occupancy");
  meFakeDigiOccupancyBarrel_ = iBooker.book2D("FakeDigiOccupancyBarrel",
                                              "Fake Digi Occupancy (Barrel);z [cm];phi",
                                              100,
                                              -30.,
                                              30.,
                                              100,
                                              -3.15,
                                              3.15);

  // HLT monitoring - trends vs lumisection
  iBooker.setCurrentFolder(topFolderName_ + "/FakeDigis/HLT");
  
  meFakeDigisPerEventVsLS_ = iBooker.bookProfile("FakeDigisPerEventVsLS",
                                                  "Fake Digis per Event vs LS;Lumisection;Fake Digis/Event",
                                                  2000,
                                                  0.,
                                                  2000.,
                                                  0.,
                                                  5000.);
  
  meFakeDigisFractionVsLS_ = iBooker.bookProfile("FakeDigisFractionVsLS",
                                                  "Fake Digis Fraction vs LS;Lumisection;Fake Fraction",
                                                  2000,
                                                  0.,
                                                  2000.,
                                                  0.,
                                                  1.0);
  
  meFakeDigisRate_ = iBooker.book1D("FakeDigisRate",
                                    "Fake Digis Rate (avg per LS);Lumisection;Rate [fake digis/event]",
                                    2000,
                                    0.,
                                    2000.);

  edm::LogInfo("SiPixelFakeDigiMonitor") << "Histograms booked in " << topFolderName_ + "/FakeDigis";
}

void SiPixelFakeDigiMonitor::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  const TrackerTopology& tTopo = iSetup.getData(trackerTopoToken_);
  const TrackerGeometry& trackerGeom = iSetup.getData(trackerGeomToken_);

  edm::Handle<SiPixelClustersHost> clustersSoA;
  iEvent.getByToken(clustersSoAToken_, clustersSoA);

  if (!clustersSoA.isValid()) {
    edm::LogWarning("SiPixelFakeDigiMonitor") << "ClustersSoA collection not valid";
    return;
  }

  edm::Handle<edm::DetSetVector<PixelDigi>> digis;
  iEvent.getByToken(digiToken_, digis);

  int totalDigis = 0;
  if (digis.isValid()) {
    for (const auto& detSet : *digis) {
      totalDigis += detSet.size();
    }
  }

  auto const& clusters_view = clustersSoA->const_view();
  const uint32_t nModules = clusters_view[0].moduleStart();

  int totalFakeDigis = 0;
  int fakeDigisBarrel = 0;
  int modulesWithFakeDigis = 0;
  std::vector<int> fakeDigisPerLayer(4, 0);
  std::vector<int> fakeDigisPerDisk(3, 0);

  for (uint32_t i = 1; i <= nModules; ++i) {
    uint32_t nFakeDigis = clusters_view[i].nFakeDigis();
    if (nFakeDigis == 0) continue;

    totalFakeDigis += nFakeDigis;
    modulesWithFakeDigis++;
    meFakeDigisPerModule_->Fill(nFakeDigis);

    uint32_t rawId = clusters_view[i].rawId();
    DetId detIdObj(rawId);
    if (detIdObj.det() != DetId::Tracker) continue;

    int subdet = detIdObj.subdetId();
    if (subdet == static_cast<int>(PixelSubdetector::PixelBarrel)) {
      meFakeDigisPerModuleBarrel_->Fill(nFakeDigis);
      fakeDigisBarrel += nFakeDigis;
      int layer = tTopo.pxbLayer(detIdObj);
      if (layer >= 1 && layer <= 4) {
        fakeDigisPerLayer[layer - 1] += nFakeDigis;
      }
    } else if (subdet == static_cast<int>(PixelSubdetector::PixelEndcap)) {
      int disk = tTopo.pxfDisk(detIdObj);
      if (disk >= 1 && disk <= 3) {
        fakeDigisPerDisk[disk - 1] += nFakeDigis;
      }
    }

    const GeomDet* geomDet = trackerGeom.idToDet(detIdObj);
    if (geomDet) {
      GlobalPoint gp = geomDet->surface().toGlobal(LocalPoint(0, 0, 0));
      if (subdet == static_cast<int>(PixelSubdetector::PixelBarrel)) {
        meFakeDigiOccupancyBarrel_->Fill(gp.z(), gp.phi());
      }
    }
  }

  // Fill event-level histograms
  meFakeDigisPerEvent_->Fill(totalFakeDigis);
  meModulesWithFakeDigis_->Fill(modulesWithFakeDigis);
  meNFakeDigisBarrel_->Fill(fakeDigisBarrel);
  
  // LS Monitoring Accumulation
  fakeDigisThisLS_ += totalFakeDigis;
  totalDigisThisLS_ += totalDigis;
  eventsThisLS_++;

  // Fill per-layer/disk histograms
  for (int i = 0; i < 4; i++) {
    if (fakeDigisPerLayer[i] > 0) {
      meNFakeDigisLayer_[i]->Fill(fakeDigisPerLayer[i]);
    }
  }
  for (int i = 0; i < 3; i++) {
    if (fakeDigisPerDisk[i] > 0) {
      meNFakeDigisDisk_[i]->Fill(fakeDigisPerDisk[i]);
    }
  }
}

DEFINE_FWK_MODULE(SiPixelFakeDigiMonitor);
