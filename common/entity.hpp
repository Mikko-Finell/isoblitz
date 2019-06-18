#ifndef entity_hpp
#define entity_hpp

#include "hitbox.hpp"
#include "animation.hpp"
#include "coordinate.hpp"
#include "observer.hpp"
#include "tile.hpp"
#include <list>

/**
 * Entity
 * Interface for entities in the gameworld.
 *
 * TODO hard nicetohave
 * need to consider how different a building
 * is from a movable agent, surely they share traits
 * such as animation, hitbox, etc; but not path. Do
 * we need seperate classes? What are the alternatives?
 */
class Entity {
public:
    Cell cell;
    Animation animation;
    Hitbox hitbox;

    virtual ~Entity();
    Entity(const std::string & name = "DEFAULT");
    void set_cell(const Cell & c);
    void clear();
    const std::string & name() const;

    std::string info() const;

private:
    std::string _name;
    float movement_cooldown = 0;
};

#endif
