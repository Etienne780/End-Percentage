#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

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
    bool init() {
        float width = 350.f;
        float height = 180.f;

        if (!geode::Popup::init(width, height)) {
            return false;
        }

        return true;
    }
};