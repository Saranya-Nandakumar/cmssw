#ifndef IOPool_Input_RootTree_h
#define IOPool_Input_RootTree_h

/*----------------------------------------------------------------------

RootTree.h // used by ROOT input sources

----------------------------------------------------------------------*/

#include "DataFormats/Provenance/interface/ProductDescription.h"
#include "DataFormats/Provenance/interface/BranchID.h"
#include "DataFormats/Provenance/interface/IndexIntoFile.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ServiceRegistry/interface/ServiceRegistryfwd.h"
#include "FWCore/Utilities/interface/BranchType.h"
#include "FWCore/Utilities/interface/InputType.h"
#include "FWCore/Utilities/interface/Signal.h"
#include "FWCore/Utilities/interface/thread_safety_macros.h"

#include "Rtypes.h"
#include "TBranch.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

class TClass;
class TTree;
class TTreeCache;

namespace edm {
  class RootDelayedReaderBase;
  class InputFile;

  namespace roottree {
    unsigned int const defaultCacheSize = 20U * 1024 * 1024;
    unsigned int const defaultNonEventCacheSize = 1U * 1024 * 1024;
    unsigned int const defaultLearningEntries = 20U;
    unsigned int const defaultNonEventLearningEntries = 1U;
    using EntryNumber = IndexIntoFile::EntryNumber_t;
    struct BranchInfo {
      BranchInfo(ProductDescription const& prod)
          : productDescription_(prod), productBranch_(nullptr), classCache_(nullptr), offsetToWrapperBase_(0) {}
      ProductDescription const productDescription_;
      void setBranch(TBranch* branch, TClass const* wrapperBaseTClass);
      std::unique_ptr<WrapperBase> newWrapper() const;
      TBranch* productBranch_;

    private:
      //All access to a ROOT file is serialized
      TClass* classCache_;
      Int_t offsetToWrapperBase_;
    };

    class BranchMap {
    public:
      using Map = std::unordered_map<unsigned int, BranchInfo>;

      void reserve(Map::size_type iSize) { map_.reserve(iSize); }
      void insert(edm::BranchID const& iKey, BranchInfo const& iInfo) { map_.emplace(iKey.id(), iInfo); }
      BranchInfo const* find(BranchID const& iKey) const { return find(iKey.id()); }
      BranchInfo const* find(unsigned int iKey) const {
        auto itFound = map_.find(iKey);
        if (itFound == map_.end()) {
          return nullptr;
        }
        return &itFound->second;
      }

      using const_iterator = Map::const_iterator;
      const_iterator begin() const { return map_.cbegin(); }
      const_iterator end() const { return map_.cend(); }
      Map::size_type size() const { return map_.size(); }

    private:
      Map map_;
    };

    Int_t getEntry(TBranch* branch, EntryNumber entryNumber);
    Int_t getEntry(TTree* tree, EntryNumber entryNumber);
    std::unique_ptr<TTreeCache> trainCache(TTree* tree,
                                           InputFile& file,
                                           unsigned int cacheSize,
                                           char const* branchNames);
  }  // namespace roottree

  class RootTree {
  public:
    using BranchMap = roottree::BranchMap;
    using EntryNumber = roottree::EntryNumber;
    struct Options {
      unsigned int treeCacheSize = 0U;
      int treeMaxVirtualSize;
      bool enablePrefetching;
      bool promptReading = false;

      Options usingDefaultNonEventOptions() const {
        return {roottree::defaultNonEventCacheSize, treeMaxVirtualSize, enablePrefetching, false};
      }
    };

    RootTree(std::shared_ptr<InputFile> filePtr,
             BranchType const& branchType,
             unsigned int nIndexes,
             Options const& options,
             unsigned int learningEntries,
             InputType inputType);

    RootTree(std::shared_ptr<InputFile> filePtr,
             BranchType const& branchType,
             std::string const& processName,
             unsigned int nIndexes,
             Options const& options,
             unsigned int learningEntries,
             InputType inputType);

    void init(std::string const& productTreeName, unsigned int maxVirtualSize, unsigned int cacheSize);

    ~RootTree();

    RootTree(RootTree const&) = delete;             // Disallow copying and moving
    RootTree& operator=(RootTree const&) = delete;  // Disallow copying and moving

    bool isValid() const;
    void numberOfBranchesToAdd(BranchMap::Map::size_type iSize) { branches_.reserve(iSize); }
    void addBranch(ProductDescription const& prod, std::string const& oldBranchName);
    void dropBranch(std::string const& oldBranchName);
    void getEntry(TBranch* branch, EntryNumber entry) const;
    void getEntryForAllBranches() const;
    void setPresence(ProductDescription& prod, std::string const& oldBranchName);

    bool next() { return ++entryNumber_ < entries_; }
    bool nextWithCache();
    bool current() const { return entryNumber_ < entries_ && entryNumber_ >= 0; }
    bool current(EntryNumber entry) const { return entry < entries_ && entry >= 0; }
    void rewind() { entryNumber_ = 0; }
    void rewindToInvalid() { entryNumber_ = IndexIntoFile::invalidEntry; }
    void close();
    bool skipEntries(unsigned int& offset);
    EntryNumber const& entryNumber() const { return entryNumber_; }
    EntryNumber const& entryNumberForIndex(unsigned int index) const;
    EntryNumber const& entries() const { return entries_; }
    void setEntryNumber(EntryNumber theEntryNumber);
    void insertEntryForIndex(unsigned int index);
    std::vector<std::string> const& branchNames() const { return branchNames_; }
    RootDelayedReaderBase* rootDelayedReader() const;
    DelayedReader* resetAndGetRootDelayedReader() const;
    template <typename T>
    void fillAux(T*& pAux) {
      auxBranch_->SetAddress(&pAux);
      auto cache = getAuxCache(auxBranch_);
      getEntryUsingCache(auxBranch_, entryNumber_, cache);
      auxBranch_->SetAddress(nullptr);
    }

    template <typename T>
    void fillBranchEntry(TBranch* branch, T*& pbuf) {
      branch->SetAddress(&pbuf);
      getEntry(branch, entryNumber_);
      branch->SetAddress(nullptr);
    }

    template <typename T>
    void fillBranchEntryMeta(TBranch* branch, EntryNumber entryNumber, T*& pbuf) {
      if (metaTree_ != nullptr) {
        // Metadata was in separate tree.  Not cached.
        branch->SetAddress(&pbuf);
        roottree::getEntry(branch, entryNumber);
      } else {
        fillBranchEntry<T>(branch, entryNumber, pbuf);
      }
    }

    template <typename T>
    void fillBranchEntry(TBranch* branch, EntryNumber entryNumber, T*& pbuf) {
      branch->SetAddress(&pbuf);
      getEntry(branch, entryNumber);
    }

    TTree const* tree() const { return tree_; }
    TTree* tree() { return tree_; }
    TTree const* metaTree() const { return metaTree_; }
    TTree* metaTree() { return metaTree_; }
    BranchMap const& branches() const;

    //For backwards compatibility
    TBranch* branchEntryInfoBranch() const { return branchEntryInfoBranch_; }

    inline TTreeCache* checkTriggerCache(TBranch* branch, EntryNumber entryNumber) const;
    TTreeCache* checkTriggerCacheImpl(TBranch* branch, EntryNumber entryNumber) const;
    inline TTreeCache* selectCache(TBranch* branch, EntryNumber entryNumber) const;
    void trainCache(char const* branchNames);
    void resetTraining() { trainNow_ = true; }

    BranchType branchType() const { return branchType_; }
    std::string const& processName() const { return processName_; }

    void setSignals(
        signalslot::Signal<void(StreamContext const&, ModuleCallingContext const&)> const* preEventReadSource,
        signalslot::Signal<void(StreamContext const&, ModuleCallingContext const&)> const* postEventReadSource);

  private:
    RootTree(std::shared_ptr<InputFile> filePtr,
             BranchType const& branchType,
             unsigned int nIndexes,
             unsigned int learningEntries,
             bool enablePrefetching,
             bool promptRead,
             InputType inputType);

    std::shared_ptr<TTreeCache> createCacheWithSize(unsigned int cacheSize) const;
    void setCacheSize(unsigned int cacheSize);
    void setTreeMaxVirtualSize(int treeMaxVirtualSize);
    void startTraining();
    void stopTraining();
    void getEntryUsingCache(TBranch* branch, EntryNumber entry, TTreeCache*) const;
    TTreeCache* getAuxCache(TBranch* auxBranch) const;

    std::shared_ptr<InputFile> filePtr_;
    // We use bare pointers for pointers to some ROOT entities.
    // Root owns them and uses bare pointers internally.
    // Therefore,using smart pointers here will do no good.
    TTree* tree_ = nullptr;
    TTree* metaTree_ = nullptr;
    BranchType branchType_;
    std::string processName_;
    TBranch* auxBranch_ = nullptr;
    // We use a smart pointer to own the TTreeCache.
    // Unfortunately, ROOT owns it when attached to a TFile, but not after it is detached.
    // So, we make sure to it is detached before closing the TFile so there is no double delete.
    std::shared_ptr<TTreeCache> treeCache_;
    std::shared_ptr<TTreeCache> rawTreeCache_;
    CMS_SA_ALLOW mutable std::shared_ptr<TTreeCache> auxCache_;
    //All access to a ROOT file is serialized
    CMS_SA_ALLOW mutable std::shared_ptr<TTreeCache> triggerTreeCache_;
    CMS_SA_ALLOW mutable std::shared_ptr<TTreeCache> rawTriggerTreeCache_;
    CMS_SA_ALLOW mutable std::unordered_set<TBranch*> trainedSet_;
    CMS_SA_ALLOW mutable std::unordered_set<TBranch*> triggerSet_;
    EntryNumber entries_ = 0;
    EntryNumber entryNumber_ = IndexIntoFile::invalidEntry;
    std::unique_ptr<std::vector<EntryNumber> > entryNumberForIndex_;
    std::vector<std::string> branchNames_;
    BranchMap branches_;
    bool trainNow_ = false;
    EntryNumber switchOverEntry_ = -1;
    CMS_SA_ALLOW mutable EntryNumber rawTriggerSwitchOverEntry_ = -1;
    CMS_SA_ALLOW mutable bool performedSwitchOver_ = false;
    unsigned int learningEntries_;
    unsigned int cacheSize_ = 0;
    unsigned long treeAutoFlush_ = 0;
    // Enable asynchronous I/O in ROOT (done in a separate thread).  Only takes
    // effect on the primary treeCache_; all other caches have this explicitly disabled.
    bool enablePrefetching_;
    bool enableTriggerCache_;
    bool promptRead_;
    std::unique_ptr<RootDelayedReaderBase> rootDelayedReader_;

    TBranch* branchEntryInfoBranch_ = nullptr;  //backwards compatibility
    // below for backward compatibility
    TTree* infoTree_ = nullptr;  // backward compatibility
  };
}  // namespace edm
#endif
