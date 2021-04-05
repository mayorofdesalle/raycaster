#include "Entities.hpp"

Entities::Entity::Entity(float radius, size_t color, sf::Vector2f v)
    {
        position = v;
        body.setPosition(v);
        body.setRadius(radius);
        body.setFillColor(PALETTE[color]);
        body.setOrigin(radius, radius);
    }

Entities::Entity::Entity(const Entity &other)
    {
        this->position = other.position;
        this->body = other.body;
    }

Entities::Entity &Entities::Entity::operator=(const Entity &other)
    {
        if (this == &other)
		    return *this;
            
        this->position = other.position;
        this->body = other.body;
        return *this;
    }

void Entities::Entity::move(sf::Vector2f &v)
    {
        if (position.x + body.getRadius() + v.x < WINDOW_X &&
            position.x - body.getRadius() + v.x > 0)
            position.x += v.x;

        if (position.y + body.getRadius() + v.y < WINDOW_Y &&
            position.y - body.getRadius() + v.y > 0)
            position.y += v.y;

        body.setPosition(position);
    }

Entities::Entities()
    {
        dummies = std::make_unique<std::vector<Entity>>();
        collisions = std::make_unique<std::vector<sf::CircleShape>>();
        lines.setPrimitiveType(sf::Lines);
    }

void Entities::createEntity(bool entity_type, sf::Vector2f v)
    {
        if (entity_type && !player)
            player = std::make_unique<Entity>(PLAYER_SIZE, PLAYER_COLOR, v);
        else
            dummies->emplace_back(DUMMY_SIZE, DUMMY_COLOR, v);
    }

void Entities::deleteEntity(sf::Vector2f &v)
    {
        for (size_t i {}; i < dummies->size(); ++i) {
            sf::Vector2f pos = dummies->at(i).position;
            float radius = dummies->at(i).body.getRadius();
            if (v.x < pos.x + radius && v.x > pos.x - radius &&
                v.y < pos.y + radius && v.y > pos.y - radius)
                {
                    dummies->erase(dummies->begin()+i);
                }
        }
    }

void Entities::movePlayer(sf::Vector2f &v)
    {
        player->move(v);
    }

void Entities::swapPlayer(sf::Vector2f &v)
    {
        for (auto &d: *dummies) {
            if (v.x < d.position.x + d.body.getRadius() && v.x > d.position.x - d.body.getRadius() &&
                v.y < d.position.y + d.body.getRadius() && v.y > d.position.y - d.body.getRadius()) {
                    Entity temp {*player};
                    *player = d;
                    player->body.setFillColor(PALETTE[PLAYER_COLOR]);
                    temp.body.setFillColor(PALETTE[DUMMY_COLOR]);
                    d = temp;
                    break;
                }
        }
    }

void Entities::castRays(bool ray_toggle, TileMap &tilemap)
    {   
        if (ray_toggle) {
            lines.resize((dummies->size())*2);
            collisions->clear();
            collisions->reserve(dummies->size());
            for (size_t i = 0; i < dummies->size(); ++i) {

                // get a pointer to the current line
                sf::Vertex *line = &lines[i*2];
                line[0].position = player->position;
                line[0].color = PALETTE[PLAYER_RAY_COLOR];
                
                Entity d = dummies->at(i);

                sf::Vector2f rayDir {d.position.x - line[0].position.x,
                                     d.position.y - line[0].position.y};
                float len = std::sqrt(std::pow(rayDir.x, 2) + std::pow(rayDir.y, 2));
                rayDir.x /= len;
                rayDir.y /= len;

                // sf::Vector2f rayUnitStep {std::fabs(1.0 / rayDir.x), std::fabs(1.0 / rayDir.y)};
                sf::Vector2f rayUnitStep {};
                rayUnitStep.x = std::sqrt(1 + std::pow(rayDir.y / rayDir.x, 2));
                rayUnitStep.y = std::sqrt(1 + std::pow(rayDir.x / rayDir.y, 2));
                sf::Vector2i mapCheck;
                mapCheck.x = line[0].position.x;
                mapCheck.y = line[0].position.y;
                sf::Vector2f rayLen1D {};
                sf::Vector2i step {};

                // establish starting conditions
                if (rayDir.x < 0)
                {
                    step.x = -1;
                    rayLen1D.x = (line[0].position.x - mapCheck.x) * rayUnitStep.x;
                }
                else
                {
                    step.x = 1;
                    rayLen1D.x = (mapCheck.x - line[0].position.x) * rayUnitStep.x;
                }

                if (rayDir.y < 0)
                {
                    step.y = -1;
                    rayLen1D.y = (line[0].position.y - mapCheck.y) * rayUnitStep.y;
                }
                else
                {
                    step.y = 1;
                    rayLen1D.y = (mapCheck.y - line[0].position.y) * rayUnitStep.y;
                }
 
                // perform "Walk" until collision or range check
                bool tileFound {false};
                float distance {};
                while (!tileFound && distance < MAX_RAY_DISTANCE)
                {
                    // walk along shortest path
                    if (rayLen1D.x < rayLen1D.y)
                    {
                        mapCheck.x += step.x;
                        distance = rayLen1D.x;
                        rayLen1D.x += rayUnitStep.x;
                    }
                    else
                    {
                        mapCheck.y += step.y;
                        distance = rayLen1D.y;
                        rayLen1D.y += rayUnitStep.y;
                    }

                    // test tile at new test point
                    if (mapCheck.x >= 0 && mapCheck.x < WINDOW_X && mapCheck.y >= 0 && mapCheck.y < WINDOW_Y)
                    {
                        size_t i = mapCheck.x/GRID_SIZE;
                        size_t j = mapCheck.y/GRID_SIZE;
                        size_t width = WINDOW_X/GRID_SIZE;
                        sf::Vertex *quad = &tilemap.getTiles()[(i + j * width) * 4];
                        if (quad[0].color == PALETTE[GRID_ACTIVE_COLOR])
                            tileFound = true;
                    }
                }

                // Calculate collision location
                if (tileFound)
                {
                    line[1].position = line[0].position + rayDir * distance;
                    sf::CircleShape collision (GRID_SIZE/4);
                    collision.setFillColor(sf::Color::Transparent);
                    collision.setPosition(line[1].position);
                    collision.setOrigin(collision.getRadius(), collision.getRadius());
                    collision.setOutlineColor(PALETTE[COLLISION_COLOR]);
                    collision.setOutlineThickness(collision.getRadius()/2);
                    collisions->emplace_back(collision);
                } else
                    line[1].position = d.position;
                    
                line[1].color = PALETTE[DUMMY_RAY_COLOR];
            }
        }
        else {
            collisions->clear();
            lines.clear();
        }
    }

void Entities::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // draw dummies
        for (auto &n: *dummies)
            target.draw(n.body, states);

        // draw player
        target.draw(player->body, states);

        // draw rays
        target.draw(lines, states);

        // draw collisions
        for (auto &c: *collisions)
            target.draw(c, states);
    }
