#include "TileMap.hpp"

void changeColor(sf::Vertex *v, size_t color);

TileMap::TileMap()
    {
        size_t width {WINDOW_X/GRID_SIZE};
        size_t height {WINDOW_Y/GRID_SIZE};
        tiles.setPrimitiveType(sf::Quads);
        tiles.resize(width * height * 4);

        for (size_t i = 0; i < width; ++i)
            for (size_t j = 0; j < height-2; ++j)
            {
                // get a pointer to the current tile's quad
                sf::Vertex *quad = &tiles[(i + j * width) * 4];

                // define its 4 corners and change their colors
                quad[0].position = sf::Vector2f(i * GRID_SIZE, j * GRID_SIZE);
                quad[1].position = sf::Vector2f((i + 1) * GRID_SIZE, j * GRID_SIZE);
                quad[2].position = sf::Vector2f((i + 1) * GRID_SIZE, (j + 1) * GRID_SIZE);
                quad[3].position = sf::Vector2f(i * GRID_SIZE, (j + 1) * GRID_SIZE);
                changeColor(quad, (((i + j) % 2 == 0) ? GRID_COLOR1 : GRID_COLOR2));
                
            }
    }

void TileMap::toggleTile(sf::Vector2f v, bool build_mode)
    {
        size_t width {WINDOW_X/GRID_SIZE};
        size_t i = v.x/GRID_SIZE;
        size_t j = v.y/GRID_SIZE;
        sf::Vertex *quad = &tiles[(i + j * width) * 4];
        if (build_mode) changeColor(quad, GRID_ACTIVE_COLOR);
        else changeColor(quad, (((i + j) % 2 == 0) ? GRID_COLOR1 : GRID_COLOR2));
    }

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // draw the vertex array
        target.draw(tiles, states);
    }

void changeColor(sf::Vertex *v, size_t color)
    {
        for (size_t i {}; i < 4; i++)
            v[i].color = sf::Color{PALETTE[color].r,
                                    PALETTE[color].g,
                                    PALETTE[color].b,
                                    PALETTE[color].a};
    }

sf::VertexArray &TileMap::getTiles() {
    return tiles;
}