#include "UI.hpp"  

UI::UI() : block_mode_text{"block", font, GRID_SIZE/2},
           block_mode{"place", font, GRID_SIZE},
           entity_mode_text{"entity", font, GRID_SIZE/2},
           entity_mode{"create", font, GRID_SIZE}
    {
        font.loadFromFile("res/creteround.ttf");
        cursor.loadFromSystem(sf::Cursor::Cross);
        block_mode_text.setFillColor(sf::Color::White);
        block_mode_text.setPosition(sf::Vector2f{GRID_SIZE, ((WINDOW_Y/GRID_SIZE) - 1) * GRID_SIZE + GRID_SIZE/6});
        block_mode.setPosition(sf::Vector2f{GRID_SIZE, ((WINDOW_Y/GRID_SIZE) - 2) * GRID_SIZE + GRID_SIZE/6});
        block_mode.setCharacterSize(3*GRID_SIZE/4);
        entity_mode_text.setFillColor(sf::Color::White);
        entity_mode_text.setPosition(sf::Vector2f{GRID_SIZE*4, ((WINDOW_Y/GRID_SIZE) - 1) * GRID_SIZE + GRID_SIZE/6});
        entity_mode.setPosition(sf::Vector2f{GRID_SIZE*4, ((WINDOW_Y/GRID_SIZE) - 2) * GRID_SIZE + GRID_SIZE/6});
        entity_mode.setCharacterSize(3*GRID_SIZE/4);
    }

void UI::update(bool block_toggle, bool entity_toggle)
    {   
        block_mode.setString((block_toggle ? "place" : "remove"));
        entity_mode.setString((entity_toggle ? "create" : "delete"));
    }

void UI::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // draw components
        target.draw(block_mode_text);
        target.draw(block_mode);
        target.draw(entity_mode_text);
        target.draw(entity_mode);
    }