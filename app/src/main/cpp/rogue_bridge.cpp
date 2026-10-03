#include "rogue_bridge.h"
#include <utility>

namespace rogue25d {
void RogueBridge::setStoragePath(std::string path){ storagePath_=std::move(path); }
const std::string& RogueBridge::storagePath() const { return storagePath_; }
void RogueBridge::setButtons(std::uint32_t buttons){ buttons_=buttons; }
std::uint32_t RogueBridge::buttons() const { return buttons_; }
void RogueBridge::setPlayerState(PlayerState state){ player_=state; }
PlayerState RogueBridge::playerState() const { return player_; }
RogueBridge& bridge(){ static RogueBridge instance; return instance; }
}
