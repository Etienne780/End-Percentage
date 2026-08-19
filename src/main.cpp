#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GameLevelmanager.hpp>

#include "./EPSettingsPopup.hpp"
#include "./EPSettings.hpp"

using namespace geode::prelude;

// ----- Pause-Menu-Button and Popup ------------------------
class $modify(EPPauseLayer, PauseLayer) {

    void customSetup() {
        PauseLayer::customSetup();

        auto sideMenu = this->getChildByID("left-button-menu");
        if (!sideMenu) 
            return;

        auto sprite = CCSprite::create("end_percentageBtn.png"_spr);
        sprite->setPosition(sprite->getContentSize() / 2);
        sprite->setScale(0.75f);
        auto button = CCMenuItemSpriteExtra::create(
            sprite,
            nullptr,
            this,
            menu_selector(EPPauseLayer::onEPPopupOpened)
        );

        button->setID("ep-end-percentage-button");
        sideMenu->addChild(button);
        sideMenu->updateLayout();
    }

    void onEPPopupOpened(CCObject*) {
        auto playLayer = PlayLayer::get();

        if (!playLayer || !playLayer->m_level)
            return;

        auto* level = playLayer->m_level;
        auto popup = EPSettingsPopup::create(level);
        popup->show();
    }

};

// ----- Display corrected percentage ------------------------
class $modify(EPPlayLayer, PlayLayer) {
public:
    struct Fields {
        bool getSettings = true;
        bool hasSettings = false;
        LevelEndPercentage currentSettings;
    };

    void onExit() {
        m_fields->getSettings = true;
        PlayLayer::onExit();
    }

    float getCurrentPercent() {
        float real = PlayLayer::getCurrentPercent();

        if (!m_level) {
            m_fields->hasSettings = false;
            m_fields->getSettings = false;
            return real;
        }

        if (m_fields->getSettings) {
            auto result = getLevelSettings(m_level);

            if (result) {
                m_fields->currentSettings = *result;
                m_fields->hasSettings = true;
            }
            else {
                m_fields->currentSettings = LevelEndPercentage{};
                m_fields->hasSettings = false;
            }
        
            m_fields->getSettings = false;
        }

        if (!m_fields->hasSettings)
            return real;

        const auto& settings = m_fields->currentSettings;

        if (!settings.enabled || settings.percentage <= 0.f)
            return real;

        float scaled = real / static_cast<float>(settings.percentage) * 100.f;
        return std::min(scaled, 100.f);
    }
};

// ----- free deleted levels ------------------------
class $modify(EPGameLevelmanager, GameLevelManager) {
public:
    void deleteLevel(GJGameLevel* level) {
        if (level) {
            auto deleted = deleteLevelSettings(level);

            log::info(
                "Deleting level '{}': settings {}",
                level->m_levelName,
                deleted ? "removed" : "not found"
            );
        }

        GameLevelManager::deleteLevel(level);
    }
};