#include <cstdint>
#include <memory>
#include <vector>
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/SiPixelDigiSoA/interface/SiPixelDigisSoA.h"
#include "DataFormats/SiPixelDigiSoA/interface/alpaka/SiPixelDigisSoACollection.h"
#include "DataFormats/SiPixelClusterSoA/interface/SiPixelClustersSoA.h"
#include "DataFormats/SiPixelClusterSoA/interface/alpaka/SiPixelClustersSoACollection.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/Event.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/stream/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/InputTag.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE {

class SiPixelClusterSoAConverter : public stream::EDProducer<> {
public:
  explicit SiPixelClusterSoAConverter(const edm::ParameterSet& iConfig);
  ~SiPixelClusterSoAConverter() override = default;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void produce(device::Event& iEvent, const device::EventSetup& iSetup) override;

  const edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster>> pixelClusterToken_;
  const device::EDPutToken<SiPixelDigisSoACollection> pixelDigisSoAToken_;
  const device::EDPutToken<SiPixelClustersSoACollection> pixelClustersSoAToken_;
};

SiPixelClusterSoAConverter::SiPixelClusterSoAConverter(const edm::ParameterSet& iConfig)
    : stream::EDProducer<>(iConfig),
      pixelClusterToken_{consumes(iConfig.getParameter<edm::InputTag>("pixelClusterSource"))},
      pixelDigisSoAToken_{produces()},
      pixelClustersSoAToken_{produces()} {}

void SiPixelClusterSoAConverter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("pixelClusterSource", edm::InputTag("siPixelClusters"));
  descriptions.addWithDefaultLabel(desc);
}

void SiPixelClusterSoAConverter::produce(device::Event& iEvent, const device::EventSetup& iSetup) {
  auto const& pixelClusters = iEvent.get(pixelClusterToken_);
  auto queue = iEvent.queue();

  // Count the total number of digis and clusters
  uint32_t totalDigis = 0;
  uint32_t totalClusters = 0;
  for (auto const& detSet : pixelClusters) {
    totalDigis += detSet.size() * detSet[0].size(); // Assuming each cluster has multiple digis
    totalClusters += detSet.size();
  }

  // Allocate memory for the digis and clusters SoA collections
  SiPixelDigisSoACollection pixelDigisSoA(totalDigis, queue);
  SiPixelClustersSoACollection pixelClustersSoA(totalClusters, queue);

  // Fill the digis and clusters SoA collections
  uint32_t digiIndex = 0;
  uint32_t clusterIndex = 0;
  uint32_t moduleStart = 0;
  uint32_t clusModuleStart = 0;

  for (auto const& detSet : pixelClusters) {
    uint32_t moduleId = detSet.detId(); // Assuming detId is used as moduleId
    uint32_t clusInModule = detSet.size();

    // Fill the clusters SoA
    pixelClustersSoA.view()[clusterIndex].moduleStart() = moduleStart;
    pixelClustersSoA.view()[clusterIndex].clusInModule() = clusInModule;
    pixelClustersSoA.view()[clusterIndex].moduleId() = moduleId;
    pixelClustersSoA.view()[clusterIndex].clusModuleStart() = clusModuleStart;

    // Fill the digis SoA
    for (auto const& cluster : detSet) {
      for (uint16_t i = 0; i < cluster.size(); ++i) {
        auto const& pixel = cluster.pixel(i);
        pixelDigisSoA.view()[digiIndex].clus() = clusterIndex;
        pixelDigisSoA.view()[digiIndex].rawIdArr() = moduleId;
        pixelDigisSoA.view()[digiIndex].adc() = pixel.adc;
        pixelDigisSoA.view()[digiIndex].xx() = pixel.x;
        pixelDigisSoA.view()[digiIndex].yy() = pixel.y;
        pixelDigisSoA.view()[digiIndex].moduleId() = moduleId;
        digiIndex++;
      }
      clusModuleStart += 1;
    }

    moduleStart += clusInModule;
    clusterIndex++;
  }

  // Make the SoA collections available in the event
  iEvent.emplace(pixelDigisSoAToken_, std::move(pixelDigisSoA));
  iEvent.emplace(pixelClustersSoAToken_, std::move(pixelClustersSoA));
}

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE

#include "HeterogeneousCore/AlpakaCore/interface/alpaka/MakerMacros.h"
DEFINE_FWK_ALPAKA_MODULE(SiPixelClusterSoAConverter);