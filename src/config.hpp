#ifndef CONFIG_HPP
#define CONFIG_HPP
#pragma once

// WINDOW DIMENSIONS
const size_t WINDOW_X {1024};
const size_t WINDOW_Y {768};

// SIZES
const size_t GRID_SIZE {32};
const size_t PLAYER_SIZE {16};
const size_t DUMMY_SIZE {16};

// SPEEDS
const size_t PLAYER_SPEED {PLAYER_SIZE*GRID_SIZE};

// MAX RAY DISTANCE
const size_t MAX_RAY_DISTANCE {(WINDOW_X > WINDOW_Y) ? WINDOW_Y : WINDOW_X};

// COLOR PALETTE
const std::vector<sf::Color> PALETTE {
    sf::Color{1, 22, 39, 255},        // grid #1
    sf::Color{24, 45, 62, 255},
    sf::Color{131, 56, 236, 255},
    sf::Color{46, 196, 182, 255},
    sf::Color{255, 159, 28, 255},
    sf::Color{196, 81, 46, 255},
    sf::Color{62, 28, 255, 255},
    sf::Color{253, 255, 252, 255}
};

enum Color {
    GRID_COLOR1,
    GRID_COLOR2,
    GRID_ACTIVE_COLOR,
    PLAYER_COLOR,
    DUMMY_COLOR,
    PLAYER_RAY_COLOR,
    DUMMY_RAY_COLOR,
    COLLISION_COLOR
};

#endif