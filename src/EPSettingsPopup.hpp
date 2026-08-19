#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/ui/Button.hpp>

#include "./EPSettings.hpp"

using namespace geode::prelude;

class EPSettingsPopup : public Popup {
public:
    static EPSettingsPopup* create(GJGameLevel* level) {
        auto popup = new EPSettingsPopup{};
        if (popup->init(level)) {
            popup->autorelease();
            return popup;
        }
        delete popup;
        return nullptr;
    }

protected:
    GJGameLevel* m_currentLevel = nullptr;
    TextInput* m_input = nullptr;
    CCMenuItemToggler* m_inputToggle = nullptr;
    LevelEndPercentage m_initSettings;

    bool init(GJGameLevel* level) {
        if (!initPopup())
            return false;

        m_currentLevel = level;
        auto result = getLevelSettings(level);
        if (result) {
            m_initSettings = *result;
        } else {
            m_initSettings = LevelEndPercentage{};
        }

        resetElements(&m_initSettings);

        return true;
    }

    bool initPopup() {
        constexpr float width = 300.f;
        constexpr float height = 160.f;

        constexpr float containerWidth = 250.f;
        constexpr float containerHeight = 100.f;

        if (!geode::Popup::init(width, height))
            return false;

        this->setTitle("End Percentage");

        auto* container = CCNode::create();
        container->setID("ep-settings-popup-container");
        container->setContentSize({
            containerWidth,
            containerHeight
        });

        container->setLayout(
            ColumnLayout::create()
                ->setGap(10.f)
                ->setAxisReverse(true)
                ->setAxisAlignment(AxisAlignment::Center)
                ->setCrossAxisAlignment(AxisAlignment::Center)
        );

        auto* desc = geode::Label::create("End the level at:", "bigFont.fnt");
        desc->setID("ep-settings-popup-container-desc");
        container->addChild(desc);

        auto* inputBox = CCMenu::create();
        inputBox->setID("ep-settings-popup-container-input-box");
        inputBox->setContentSize({containerWidth, 50.f });
        inputBox->setLayout(
            RowLayout::create()
                ->setGap(10.f)
                ->setAxisAlignment(AxisAlignment::Center)
                ->setCrossAxisAlignment(AxisAlignment::Center)
        );

        m_input = geode::TextInput::create(100.f, "Percentage", "bigFont.fnt");
        m_input->setFilter("0123456789.");
        m_input->setMaxCharCount(5);

        inputBox->addChild(m_input);

        m_inputToggle = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(EPSettingsPopup::onInputToggle),
            0.8f
        );

        inputBox->addChild(m_inputToggle);

        inputBox->updateLayout();
        container->addChild(inputBox);

        auto resetMenu = CCMenu::create();
        resetMenu->setID("ep-settings-popup-container-reset-menu");
        
        auto resetSpr = ButtonSprite::create("Reset");
        auto resetBtn = CCMenuItemSpriteExtra::create(
            resetSpr,
            this,
            menu_selector(EPSettingsPopup::onResetBtn)
        );
        resetBtn->setID("ep-settings-popup-container-reset-button");
        auto resetBtnSize = resetSpr->getContentSize();
        resetBtn->setPosition(resetBtnSize.width * 0.5, resetBtnSize.height * 0.5f);

        resetMenu->setContentSize(resetBtnSize);
        resetMenu->addChild(resetBtn);

        container->addChild(resetMenu);

        container->updateLayout();
        desc->setScale(0.6f);// idk scale gets reset by update

        auto conSize = container->getContentSize();
        CCPoint conOffset{ conSize.width * -0.5f, conSize.height * -0.5f };
        m_mainLayer->addChildAtPosition(
            container,
            Anchor::Center,
            conOffset
        );

        return true;
    }

    void onClose(CCObject* sender) override {
        bool enabled = m_inputToggle->isToggled();
        float percentage = 100;

        auto value = geode::utils::numFromString<float>(
            m_input->getString()
        );

        if (value) {
            percentage = std::clamp(*value, 0.f, 100.f);
        }

        if (m_initSettings.enabled != enabled ||
            m_initSettings.percentage != percentage) {
            
            LevelEndPercentage settings;
            settings.enabled = enabled;
            settings.percentage = percentage;

            setLevelSettings(m_currentLevel, settings);   
        }

        geode::Popup::onClose(sender);
    }

    void resetElements(LevelEndPercentage* settings = nullptr) {
        bool enabled = settings ? settings->enabled : false;
        float percentage = settings ? settings->percentage : 100;
        
        if (m_inputToggle)
            m_inputToggle->toggle(enabled);
        
        if (m_input) {
            m_input->setEnabled(enabled);
            m_input->setString(fmt::format("{}", percentage), false);
        }
    }

    void onInputToggle(CCObject*) {
        if (!m_input || !m_inputToggle)
            return;

        m_input->setEnabled(!m_inputToggle->isToggled());
    }

    void onResetBtn(CCObject*) {
        auto deleted = deleteLevelSettings(m_currentLevel);

        m_initSettings = LevelEndPercentage{};
        resetElements(&m_initSettings);

        log::info(
            "Deleting level '{}': settings {}",
            m_currentLevel ? m_currentLevel->m_levelName : "null level",
            deleted ? "removed" : "not found"
        );
    }

};