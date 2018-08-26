#include "entity.hpp"
#include <cmath>
#include <cassert>
#include <iostream>

namespace {
enum compassDir {
    left = 0, up_left = 1, up = 2, up_right = 3, 
    right = 4, down_right = 5, down = 6, down_left = 7
};

inline std::string vec_to_dir(const sf::Vector2f & vector) {
    // TODO check that any of this makes sense
    static const std::string headings[8] = {
        "down", "down-left", "left", "up-left", 
        "up", "up-right", "right", "down-right"
    };

    // actual conversion code:
    float angle = atan2( vector.y, vector.x );
    // add 45 degrees because of isometric rotation
    angle -= M_PI/4;
    int octant = int( 8 * angle / (2*M_PI) + 8.5 ) % 8;

    compassDir dir = (compassDir) octant;  // typecast to enum: 0 -> E etc.
    return headings[octant];
}

inline sf::Vector2f unit_vector(const sf::Vector2f & a, const sf::Vector2f & b) {
    return (b - a) / util::distance(a, b);
}
}

Entity::Entity(const uid_t & id, const type_id_t & type) 
    : uid(id), type_id(type), animation(type)
{
}

void Entity::update(time_t dt) {
    using namespace std;

    float movespeed = 200;
    static float cooldown = movespeed;
    static Position targetpos;
    static Position ourpos;
    static float velocity;
    static Position uvec;
    static std::string dir;
    static bool stopped = false;
    
    if (cell != target) {
        if (cooldown <= 0) {
            cooldown = movespeed;

            dir = vec_to_dir(unit_vector(cell, target));
            try {
                animation.set_sequence("move-" + dir);
            }
            catch (std::out_of_range) {
                std::cerr << type_id << ", id=" << uid << 
                    " doesn't have sequence " << "move-" + dir << std::endl;
            }

            cell = target;
            hitbox.set_position(cell.to_pixel());
            if (path.empty()) {
                stopped = true;
            }
            else {
                stopped = true;
                target = path.front();
                path.pop_front();
            }
            ourpos = animation.sprite.get_origin();
            targetpos = cell.to_pixel();
            auto dist = util::distance(cell.to_pixel(), target.to_pixel());
            velocity = dist / movespeed;
            uvec = (targetpos - ourpos) / dist;
        }
        else {
            cooldown -= dt;
        }

        if (ourpos != targetpos) {
            auto dist = util::distance(ourpos, animation.sprite.get_origin());
            if (dist < 1.0f) {
                animation.sprite.set_position(targetpos);
            }
            else {
                ourpos += uvec * velocity * dt;
                animation.sprite.set_position(ourpos);
            }
        }
    }
    else { // cell == target
        if (stopped) {
            std::cout << "Stopped\n" << info() << std::endl;
            stopped = false;
            try {
                animation.set_sequence("idle-" + dir);
            }
            catch (std::out_of_range) {
                std::cerr << type_id << ", id=" << uid << 
                    " doesn't have sequence " << "idle-" + dir << std::endl;
            }
        }
    }
}

void Entity::set_cell(const cell_t & c) {
    cell = c;
    target = c;
    auto pos = cell.to_pixel();
    hitbox.set_position(pos);
    animation.sprite.set_position(pos.x, pos.y);
}

void Entity::set_hitbox(const Hitbox & hb) {
    hitbox = hb;
}

void Entity::serialize(std::ostream & out) const {
    throw std::logic_error{"Entity::serialize not implemented"};
}

void Entity::deserialize(std::istream & in) {
    throw std::logic_error{"Entity::deserialize not implemented"};
}

// EntitySystem /////////////////////////////////////////////////////////////////

void EntitySystem::remove(GameObject * go) {
    auto entity = dynamic_cast<Entity *>(go);
    assert(entity != nullptr);
    remove(entity);
}

void EntitySystem::remove(Entity * entity) {
    entities.erase(entity);
    entity->reg(nullptr);
}

void EntitySystem::add(Entity * entity) {
    entities.insert(entity);
}

void EntitySystem::update(time_t dt) {
    for (auto & entity : entities) {
        entity->update(dt);
    }
}
