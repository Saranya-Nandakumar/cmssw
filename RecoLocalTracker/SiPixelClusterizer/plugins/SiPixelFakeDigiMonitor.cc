#include "RecoLocalTracker/SiPixelClusterizer/plugins/SiPixelFakeDigiMonitor.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/TrackerCommon/interface/PixelBarrelName.h"
#include "DataFormats/TrackerCommon/interface/PixelEndcapName.h"

SiPixelFakeDigiMonitor::SiPixelFakeDigiMonitor(const edm::ParameterSet& iConfig)
    : clustersSoASrc_(iConfig.getParameter<edm::InputTag>("clustersSoASrc")),
      digiSrc_(iConfig.getParameter<edm::InputTag>("digiSrc")),
      topFolderName_(iConfig.getParameter<std::string>("topFolderName")),
      clustersSoAToken_(consumes<SiPixelClustersHost>(clustersSoASrc_)),
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

  meFakeDigisFraction_ = iBooker.book1D("FakeDigisFraction",
                                       "Fraction of Fake Digis per Event;Fraction;Events",
                                       100,
                                       0.,
                                       1.0);

  meTotalDigisPerEvent_ = iBooker.book1D("TotalDigisPerEvent",
                                         "Total Number of Digis per Event;Total Digis;Events",
                                        1000,
                                        0.,
                                        50000.);

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

  meFakeDigisPerModuleEndcap_ = iBooker.book1D("FakeDigisPerModuleEndcap",
                                               "Fake Digis per Module (Endcap);Fake Digis;Modules",
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

  meFakeDigisPerClusterBarrel_ = iBooker.book1D("FakeDigisPerClusterBarrel",
                                                "Fake Digis per Cluster (Barrel);Fake Digis;Clusters",
                                                50,
                                                0.,
                                                50.);

  meFakeDigisPerClusterEndcap_ = iBooker.book1D("FakeDigisPerClusterEndcap",
                                                "Fake Digis per Cluster (Endcap);Fake Digis;Clusters",
                                                50,
                                                0.,
                                                50.);

  // Barrel vs Endcap
  meNFakeDigisBarrel_ = iBooker.book1D("NFakeDigisBarrel",
                                       "Number of Fake Digis (Barrel);Fake Digis;Events",
                                       500,
                                       0.,
                                       5000.);

  meNFakeDigisEndcap_ = iBooker.book1D("NFakeDigisEndcap",
                                       "Number of Fake Digis (Endcap);Fake Digis;Events",
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

  meFakeDigiOccupancyEndcap_ = iBooker.book2D("FakeDigiOccupancyEndcap",
                                              "Fake Digi Occupancy (Endcap);z [cm];r [cm]",
                                              100,
                                              -60.,
                                              60.,
                                              50,
                                              0.,
                                              20.);

  meFakeDigiDistribution_ = iBooker.book2D("FakeDigiDistribution",
                                           "Fake Digi Distribution;DetId;Event",
                                           500,
                                           0.,
                                           500.,
                                           100,
                                           0.,
                                           100.);

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
  // Get tracker topology and geometry
  const TrackerTopology& tTopo = iSetup.getData(trackerTopoToken_);
  const TrackerGeometry& trackerGeom = iSetup.getData(trackerGeomToken_);

  // Get clusters SoA (contains fake digi counts per module)
  edm::Handle<SiPixelClustersHost> clustersSoA;
  iEvent.getByToken(clustersSoAToken_, clustersSoA);

  if (!clustersSoA.isValid()) {
    edm::LogWarning("SiPixelFakeDigiMonitor") << "ClustersSoA collection not valid";
    return;
  }

  // Get digis for total count
  edm::Handle<edm::DetSetVector<PixelDigi>> digis;
  iEvent.getByToken(digiToken_, digis);

  int totalDigis = 0;
  if (digis.isValid()) {
    for (const auto& detSet : *digis) {
      totalDigis += detSet.size();
    }
  }

  // Access the SoA view
  auto const& clusters_view = clustersSoA->const_view();
  const uint32_t nModules = clusters_view[0].moduleStart();  // Total number of modules with data

  // Counters
  int totalFakeDigis = 0;
  int fakeDigisBarrel = 0;
  int fakeDigisEndcap = 0;
  int modulesWithFakeDigis = 0;
  std::vector<int> fakeDigisPerLayer(4, 0);
  std::vector<int> fakeDigisPerDisk(3, 0);

  // Loop over all modules in the SoA
  for (uint32_t i = 1; i <= nModules; ++i) {
    uint32_t moduleId = clusters_view[i].moduleId();
    uint32_t nFakeDigis = clusters_view[i].nFakeDigis();  // Fixed: use i, not moduleId

    if (nFakeDigis == 0)
      continue;

    totalFakeDigis += nFakeDigis;
    modulesWithFakeDigis++;

    // Fill per-module histograms
    meFakeDigisPerModule_->Fill(nFakeDigis);

    // We need the raw detId - this is stored in moduleId
    // moduleId contains the actual detector ID
    
    // Determine if barrel or endcap based on moduleId
    DetId detIdObj(moduleId);
    
    if (detIdObj.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {
      meFakeDigisPerModuleBarrel_->Fill(nFakeDigis);
      fakeDigisBarrel += nFakeDigis;
      
      // Per-layer counting
      int layer = tTopo.pxbLayer(detIdObj);
      if (layer >= 1 && layer <= 4) {
        fakeDigisPerLayer[layer - 1] += nFakeDigis;
      }
    } else if (detIdObj.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)) {
      meFakeDigisPerModuleEndcap_->Fill(nFakeDigis);
      fakeDigisEndcap += nFakeDigis;
      
      // Per-disk counting
      int disk = tTopo.pxfDisk(detIdObj);
      if (disk >= 1 && disk <= 3) {
        fakeDigisPerDisk[disk - 1] += nFakeDigis;
      }
    }

    // Fill occupancy maps
    try {
      const GeomDet* geomDet = trackerGeom.idToDet(detIdObj);
      if (geomDet) {
        GlobalPoint gp = geomDet->surface().toGlobal(LocalPoint(0, 0, 0));
        
        if (detIdObj.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {
          meFakeDigiOccupancyBarrel_->Fill(gp.z(), gp.phi());
        } else if (detIdObj.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)) {
          meFakeDigiOccupancyEndcap_->Fill(gp.z(), gp.perp());
        }
      }
    } catch (...) {
      // Geometry lookup failed, skip occupancy plots for this module
    }
  }

  // Fill event-level histograms
  meFakeDigisPerEvent_->Fill(totalFakeDigis);
  meTotalDigisPerEvent_->Fill(totalDigis);
  meModulesWithFakeDigis_->Fill(modulesWithFakeDigis);

  if (totalDigis > 0) {
    float fraction = static_cast<float>(totalFakeDigis) / static_cast<float>(totalDigis);
    meFakeDigisFraction_->Fill(fraction);
  }

  meNFakeDigisBarrel_->Fill(fakeDigisBarrel);
  meNFakeDigisEndcap_->Fill(fakeDigisEndcap);
  
  // Accumulate for lumisection monitoring
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

// Define this as a framework module
DEFINE_FWK_MODULE(SiPixelFakeDigiMonitor);
