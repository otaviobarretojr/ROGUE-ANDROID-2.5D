#pragma once
#include <cstdint>
#include <string>

namespace rogue25d {

enum class RogueButton : std::uint32_t {
    Up=1u<<0, Down=1u<<1, Left=1u<<2, Right=1u<<3,
    A=1u<<4, B=1u<<5, L=1u<<6, R=1u<<7,
    Start=1u<<8, Select=1u<<9
};

struct PlayerState {
    std::int32_t mapId{0};
    std::int32_t x{0};
    std::int32_t y{0};
    std::int32_t elevation{0};
    std::int32_t facing{0};
};

class RogueBridge {
public:
    void setStoragePath(std::string path);
    const std::string& storagePath() const;
    void setButtons(std::uint32_t buttons);
    std::uint32_t buttons() const;
    void setPlayerState(PlayerState state);
    PlayerState playerState() const;
private:
    std::string storagePath_;
    std::uint32_t buttons_{0};
    PlayerState player_{};
};

RogueBridge& bridge();
}
