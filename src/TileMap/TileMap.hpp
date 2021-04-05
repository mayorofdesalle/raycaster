#ifndef TILEMAP_HPP
#define TILEMAP_HPP
#pragma once

#include <SFML/Graphics.hpp>
#include "../config.hpp"

class Entities;

class TileMap : public sf::Drawable, sf::Transformable {
    friend class Entities;
    private:
        sf::VertexArray tiles;
        sf::VertexArray &getTiles();
    public:
        TileMap();
        ~TileMap() = default;
        void toggleTile(sf::Vector2f v, bool build_mode);
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif