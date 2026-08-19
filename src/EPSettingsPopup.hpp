#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/ui/Button.hpp>

using namespace geode::prelude;

class EPSettingsPopup : public Popup {
public:
    static EPSettingsPopup* create() {
        auto popup = new EPSettingsPopup{};
        if (popup->init()) {
            popup->autorelease();
            return popup;
        }
        delete popup;
        return nullptr;
    }

protected:
    TextInput* m_input = nullptr;
    CCMenuItemToggler* m_inputToggle = nullptr;

    bool init() {
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
        m_input->setString("100", false);

        inputBox->addChild(m_input);

        m_inputToggle = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(EPSettingsPopup::onInputToggle),
            0.8f
        );

        m_inputToggle->toggle(false);
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

    void onInputToggle(CCObject*) {
        if (!m_input || !m_inputToggle)
            return;

        m_input->setEnabled(!m_inputToggle->isToggled());
    }

    void onResetBtn(CCObject*) {

    }
};