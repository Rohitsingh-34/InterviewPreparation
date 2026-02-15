#pragma once
#include <string>

enum class Direction {
    UP,
    DOWN,
    IDLE
};

inline std::string ToString(Direction d) {
    switch (d) {
    case Direction::DOWN: return "DOWN";
    case Direction::UP:   return "UP";
    case Direction::IDLE: return "IDLE";
    }
    return "Unknown";
}