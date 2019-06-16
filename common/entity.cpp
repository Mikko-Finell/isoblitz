#include "entity.hpp"
#include <cmath>
#include <cassert>
#include <iostream>

namespace {
    // TODO easy
    // move this into vec_to_dir
enum compassDir {
    left = 0, up_left = 1, up = 2, up_right = 3, 
    right = 4, down_right = 5, down = 6, down_left = 7
};

inline std::string vec_to_dir(const sf::Vector2f & vector) {
    // TODO easy
    // check that any of this makes sense
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

// TODO easy
// probably move this to util
inline sf::Vector2f unit_vector(const sf::Vector2f & a, const sf::Vector2f & b) {
    return (b - a) / util::distance(a, b);
}
}

Entity::~Entity() {
    if (entitys != nullptr) {
        entitys->remove(this);
    }
}

Entity::Entity(const uid_t & id, const type_id_t & type) 
    : _uid(id), type_id(type)//, animation(type)
{
}

void Entity::update(time_t dt) {
    using namespace std;

    // TODO easy
    // make these class members
    float movespeed = 200;
    static float cooldown = movespeed;
    static Position targetpos;
    static Position spritepos;
    static float velocity;
    static Position uvec;
    static std::string dir = "down";
    static bool stopped = false;

    // TODO easy
    // create a method set_path 
    
    if (cell != target) {
        if (cooldown <= 0) {
            cooldown = movespeed;

            dir = vec_to_dir(unit_vector(cell, target));
            try {
                animation->set_sequence("move-" + dir, "Entity::update, when moving");
            }
            catch (std::out_of_range) {
                std::cerr << type_id << ", id=" << uid() << 
                    " doesn't have sequence " << "move-" + dir << std::endl;
            }

            cell = target;
            hitbox.set_position(cell.to_pixel());
            if (path.empty() == false) {
                target = path.front();
                path.pop_front();
            }

            spritepos = animation->sprite->get_origin();
            targetpos = cell.to_pixel();
            auto dist = spritepos.distance_to(targetpos);
            velocity = dist / movespeed;
            if (dist) {
                uvec = (targetpos - spritepos) / dist;
            }
            else {
                uvec = {0, 0};
            }
        }
        else {
            cooldown -= dt;
        }
    }
    if (spritepos != targetpos) {
        auto dist = targetpos.distance_to(spritepos);
        if (dist < 1.0f) {
            spritepos = targetpos;
        }
        else {
            // TODO hard nicetohave
            // check if the move is too far
            spritepos += uvec * velocity * dt; 
            animation->sprite->set_position(spritepos);
        }
        signal.position(spritepos);
    }
    else if (path.empty()) { // spritepos == targetpos
        // TODO hard nicetohave
        // this will be executed every frame when idle which is very wasteful
        // find a way to only set idle once
        try {
            animation->set_sequence("idle-" + dir, "Entity::update when path is empty");
        }
        catch (std::out_of_range) {
            std::cout << "DEBUG: Animation has the following sequences: "
              << animation->print_sequences() << std::endl;
            std::cerr << type_id << ", id=" << uid() << 
                " doesn't have sequence " << "idle-" + dir << std::endl;
            std::terminate();
        }
    }
}

void Entity::set_cell(const cell_t & c) {
    cell = c;

    // TODO hard critical
    // consider the scenario where we are in the process of traversing a path
    // and this function is called, what should happen with target/path?
    target = c;

    auto pos = cell.to_pixel();
    hitbox.set_position(pos);
    animation->sprite->set_position(pos.x, pos.y);
}

void Entity::set_hitbox(const Hitbox & hb) {
    hitbox = hb;
}

// TODO easy
// probably remove these methods since we serialize entities from editor
// by type_id and create from factory anyway
void Entity::serialize(std::ostream & out) const {
    throw std::logic_error{"Entity::serialize not implemented"};
}

void Entity::deserialize(std::istream & in) {
    throw std::logic_error{"Entity::deserialize not implemented"};
}

// EntitySystem /////////////////////////////////////////////////////////////////

void EntitySystem::remove(Entity & entity) {
    // erase(key_type) returns the number of elements removed, so we assure
    // that we are not trying to erase non-existant entities. 
    assert(entities.erase(&entity) == 1);
    entity.entitys = nullptr;
}

void EntitySystem::add(Entity * entity) {
    const bool OK = entities.insert(entity).second;
    assert(OK);
    entity->entitys = this;
}

void EntitySystem::update(time_t dt) {
    for (auto & entity : entities) {
        entity->update(dt);
    }
}
