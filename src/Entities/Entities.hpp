#ifndef ENTITIES_HPP
#define ENTITIES_HPP
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <cmath>
#include "../config.hpp"
#include "../TileMap/TileMap.hpp"

class Entities : public sf::Drawable, public sf::Transformable {
	private:
		struct Entity {
			Entity(float radius, size_t color, sf::Vector2f v);
			Entity(const Entity &other);
			~Entity() = default;
			Entity &operator=(const Entity &other);
			sf::CircleShape body;
			sf::Vector2f position;
			void move(sf::Vector2f &v);
		};
		std::unique_ptr<std::vector<Entity>> dummies;
		std::unique_ptr<Entity> player;
		std::unique_ptr<std::vector<sf::CircleShape>> collisions;
		sf::VertexArray lines;

	public:
		Entities();
		~Entities() = default;
		void createEntity(bool entity_type, sf::Vector2f v);
		void deleteEntity(sf::Vector2f &v);
		void movePlayer(sf::Vector2f &v);
		void swapPlayer(sf::Vector2f &v);
		void castRays(bool ray_toggle, TileMap &tilemap);
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

enum EntityType {
	DUMMY,
	PLAYER
};

#endif