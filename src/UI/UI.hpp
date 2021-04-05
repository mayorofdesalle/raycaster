#ifndef UI_HPP
#define UI_HPP
#pragma once

#include <SFML/Graphics.hpp>
#include "../config.hpp"

class UI : public sf::Drawable, public sf::Transformable {
	public:
		sf::Font font;
		sf::Cursor cursor;
		sf::Text block_mode_text;
		sf::Text block_mode;
		sf::Text entity_mode_text;
		sf::Text entity_mode;
		UI();
		~UI() = default;
		void update(bool block_toggle, bool entity_toggle);
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};
#endif