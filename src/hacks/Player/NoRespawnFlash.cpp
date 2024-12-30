#include <Geode/modify/PlayerObject.hpp>
#include <modules/config/config.hpp>
#include <modules/gui/gui.hpp>
#include <modules/hack/hack.hpp>

namespace eclipse::hacks::Player {

class NoRespawnFlash : public hack::Hack {
  void init() override {
    auto tab = gui::MenuTab::find("tab.player");

    tab->addToggle("player.norespawnflash")->setDescription()->handleKeybinds();
  }

  [[nodiscard]] const char* getId() const override {
    return "No Respawn Flash";
  }
};

REGISTER_HACK(NoRespawnFlash)

class $modify(NoRespawnFlashPOHook, PlayerObject){ALL_DELEGATES_AND_SAFE_PRIO(
    "player.norespawnflash") void playSpawnEffect(){}};

}  // namespace eclipse::hacks::Player
