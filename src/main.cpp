#include "TileMap/TileMap.hpp"
#include "Entities/Entities.hpp"
#include "UI/UI.hpp"
#include <SFML/System.hpp>
#include <cmath>
#include "config.hpp"

/* Environmental variables */
bool BLOCK_TOGGLE {true};
bool ENTITY_TOGGLE {true};
bool RAY_TOGGLE {false};
bool SWAP_ALLOW {true};
bool ENTITY_ALLOW {true};

void setup(sf::RenderWindow &window, UI &ui);
void handleEvents(sf::RenderWindow &window, UI &ui);
void handlePlayerMovement(TileMap &tilemap, Entities &entities, float delta);
void handleMouseActions(sf::RenderWindow &window, TileMap &tilemap, Entities &entities);
bool checkForCollision(TileMap &tilemap, Entities &entities, sf::Vector2f &v, float size);

int main () {

    sf::RenderWindow window {sf::VideoMode{WINDOW_X, WINDOW_Y}, "Raycaster"};
    UI ui;
    setup(window, ui);

    TileMap tilemap;
    Entities entities;
    entities.createEntity(PLAYER, sf::Vector2f{(1*WINDOW_X/4) - PLAYER_SIZE, (3*WINDOW_Y/4) + PLAYER_SIZE});
    entities.createEntity(DUMMY, sf::Vector2f{(3*WINDOW_X/4) + DUMMY_SIZE, (1*WINDOW_Y/4) - DUMMY_SIZE});

    sf::Clock clock;
    float delta;

    while (window.isOpen())
    {   
        delta = clock.restart().asSeconds();
        handleEvents(window, ui);
        handlePlayerMovement(tilemap, entities, delta);
        handleMouseActions(window, tilemap, entities);
        ui.update(BLOCK_TOGGLE, ENTITY_TOGGLE);
        entities.castRays(RAY_TOGGLE, tilemap);
        window.clear(PALETTE[GRID_COLOR1]);
        window.draw(tilemap);
        window.draw(entities);
        window.draw(ui);
        window.display();
    }
}

void setup(sf::RenderWindow &window, UI &ui)
    {
        window.setFramerateLimit(60);
        window.setKeyRepeatEnabled(false);
        window.setMouseCursor(ui.cursor);
    }

void handleEvents(sf::RenderWindow &window, UI &ui)
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Key::Num1) {
                    BLOCK_TOGGLE = BLOCK_TOGGLE ? false : true;
                }
                if (event.key.code == sf::Keyboard::Key::Num2) {
                    ENTITY_TOGGLE = ENTITY_TOGGLE ? false : true;
                }
                if (event.key.code == sf::Keyboard::Key::Space) {
                    RAY_TOGGLE = RAY_TOGGLE ? false : true;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Button::Middle) {
                    SWAP_ALLOW = true;
                }
                if (event.mouseButton.button == sf::Mouse::Button::Left) {
                    ENTITY_ALLOW = true;
                }
            }
            if (event.type == sf::Event::GainedFocus)
                window.setMouseCursor(ui.cursor);
            
        }
    }

void handlePlayerMovement(TileMap &tilemap, Entities &entities, float delta)
    {
        sf::Vector2f m {0, 0};

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            m.y -= 1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
                 !sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            m.y += 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            m.x -= 1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
                 !sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            m.x += 1;
        }

        double len {std::sqrt(std::pow(m.x, 2) + std::pow(m.y, 2))};
        m.x = (m.x/len) * delta * PLAYER_SPEED;
        m.y = (m.y/len) * delta * PLAYER_SPEED;
        if (len) entities.movePlayer(m);
    }

void handleMouseActions(sf::RenderWindow &window, TileMap &tilemap, Entities &entities)
    {   
        // Tile toggle
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            sf::Vector2f mouse_pos {sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y};
            if (mouse_pos.x < WINDOW_X && mouse_pos.y < (WINDOW_Y - GRID_SIZE*2))
                tilemap.toggleTile(mouse_pos, BLOCK_TOGGLE);   
        }
        
        // Entity creation
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && ENTITY_ALLOW) {
            ENTITY_ALLOW = false;
            sf::Vector2f mouse_pos {sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y};
            if (mouse_pos.x < WINDOW_X && mouse_pos.y < WINDOW_Y)
                ENTITY_TOGGLE ? entities.createEntity(DUMMY, mouse_pos) : entities.deleteEntity(mouse_pos);
        }

        // Entity swap
        if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && SWAP_ALLOW) {
            SWAP_ALLOW = false;
            sf::Vector2f mouse_pos {sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y};
            if (mouse_pos.x < WINDOW_X && mouse_pos.y < WINDOW_Y)
                entities.swapPlayer(mouse_pos);
        }
    }