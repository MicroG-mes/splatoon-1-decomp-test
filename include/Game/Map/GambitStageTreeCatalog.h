#pragma once

#include "types.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace Game {

struct StageTreeNode {
    std::string name;
    std::string filePath;
    std::string location;
};

class GambitStageTreeCatalog {
public:
    GambitStageTreeCatalog();
    ~GambitStageTreeCatalog();

    bool loadFromSzs(const char* szsPath);
    void clear();

    bool isLoaded() const { return mIsLoaded; }
    size_t getStageCount() const { return mStages.size(); }
    const StageTreeNode* getStage(size_t index) const;
    const StageTreeNode* findStageByName(const std::string& name) const;
    std::vector<const StageTreeNode*> findByLocationPrefix(const std::string& prefix) const;

    static GambitStageTreeCatalog* instance();

private:
    bool mIsLoaded = false;
    std::vector<StageTreeNode> mStages;
    std::unordered_map<std::string, size_t> mNameMap;
    static std::unique_ptr<GambitStageTreeCatalog> sInstance;
};

} // namespace Game
