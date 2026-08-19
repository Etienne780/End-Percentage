#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(EndPercentagePauseLayer, PauseLayer) {

    void customSetup() {
        PauseLayer::customSetup();

        log::info("Pause layer");
    }

};