#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Mod.hpp>
#include <matjson.hpp>
#include <unordered_map>
#include <functional>
#include <string_view>

#include <cvolton.level-id-api/include/EditorIDs.hpp>

using namespace geode::prelude;

struct LevelEndPercentage {
    bool enabled = false;
    float percentage = 100.f;
};

using LevelSettings_key = std::string;
using LevelSettings_map = std::unordered_map<LevelSettings_key, LevelEndPercentage>;

constexpr std::string_view storageSettingsKey{ "level-end-percentages" };

constexpr std::string_view jsonSettingsEnabledKey{ "enabled" };
constexpr std::string_view jsonSettingsPercentageKey{ "percentage" };

// TYPE (GJLevelType):
// 1 = robtop (built in)
// 2 = local (creator tab)
// 3 = online (uploaded to servers)
enum class EndPercentageLevelType {
    BUILT_IN = 1,
    LOCAL = 2,
    ONLINE = 3,
};

inline EndPercentageLevelType gjLevelTypeToLevelType(GJLevelType type) {
    return static_cast<EndPercentageLevelType>(type);
}

template <>
struct matjson::Serialize<LevelEndPercentage> {
    static matjson::Value toJson(LevelEndPercentage const& value) {
        auto obj = matjson::Value::object();
        obj[jsonSettingsEnabledKey] = value.enabled;
        obj[jsonSettingsPercentageKey] = static_cast<double>(value.percentage);
        return obj;
    }

    static Result<LevelEndPercentage> fromJson(matjson::Value const& value) {
        LevelEndPercentage entry;

        entry.enabled = value[jsonSettingsEnabledKey].asBool().unwrapOr(false);

        float per = static_cast<float>(value[jsonSettingsPercentageKey].asDouble().unwrapOr(100.0));
        entry.percentage = std::clamp(per, 0.f, 100.f);

        return Ok(entry);
    }
};

inline LevelSettings_key levelSettingsKey(GJGameLevel* level) {
    if (!level)
        return "";

    int levelID = EditorIDs::getID(level);    
    switch (gjLevelTypeToLevelType(level->m_levelType)) {
    case EndPercentageLevelType::BUILT_IN:
        return fmt::format("builtIn:{}", levelID);

    case EndPercentageLevelType::LOCAL:
        return fmt::format("local:{}", levelID);

    case EndPercentageLevelType::ONLINE:
        return fmt::format("online:{}", levelID);

    default:
        return "";
    }
}

inline Result<LevelEndPercentage> getLevelSettings(GJGameLevel* level) {
    if (!level)
        return Err("Level is null");

    auto key = levelSettingsKey(level);

    if (key.empty())
        return Err("Level has no valid settings key");

    auto all = Mod::get()->getSavedValue<LevelSettings_map>(storageSettingsKey);

    auto it = all.find(key);

    if (it != all.end())
        return Ok(it->second);

    return Err("No settings found for level");
}

inline void setLevelSettings(GJGameLevel* level, LevelEndPercentage settings) {
    if (!level)
        return;

    auto key = levelSettingsKey(level);
    if (key.empty())
        return;

    auto all = Mod::get()->getSavedValue<LevelSettings_map>(storageSettingsKey);

    all[key] = std::move(settings);
    Mod::get()->setSavedValue(storageSettingsKey, all);
}

inline bool deleteLevelSettings(GJGameLevel* level) {
    if (!level)
        return false;
    
    LevelSettings_key key = levelSettingsKey(level);
    if (key.empty()) 
        return false;

    auto all = Mod::get()->getSavedValue<LevelSettings_map>(storageSettingsKey);

    auto it = all.find(key);
    if (it == all.end())
        return false;

    all.erase(it);
    Mod::get()->setSavedValue(storageSettingsKey, all);
    return true;
}