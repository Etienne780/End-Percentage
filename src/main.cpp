#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include "./EPSettingsPopup.hpp"

using namespace geode::prelude;

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