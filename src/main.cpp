#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

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
        auto popup = EPSettingsPopup::create();
        popup->show();
    }

};