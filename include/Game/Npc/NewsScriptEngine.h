#pragma once

#include "types.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace Game {

struct NewsCommandEntry {
    std::string name;
    std::string subState;
};

struct NewsScenario {
    std::string newsType;
    std::vector<NewsCommandEntry> commands;
};

class NewsScriptEngine {
public:
    NewsScriptEngine();
    ~NewsScriptEngine();

    bool loadFromSzs(const char* szsPath);
    bool load(const u8* bymlData, size_t bymlSize);
    void clear();

    bool isLoaded() const { return mIsLoaded; }
    size_t getScenarioCount() const { return mScenarios.size(); }
    const NewsScenario* getScenario(size_t index) const;
    const NewsScenario* findScenario(const std::string& newsType) const;
    bool hasScenario(const std::string& newsType) const;

    static NewsScriptEngine* instance();

private:
    bool mIsLoaded = false;
    std::vector<NewsScenario> mScenarios;
    std::unordered_map<std::string, size_t> mScenarioMap;
    static std::unique_ptr<NewsScriptEngine> sInstance;
};

} // namespace Game
