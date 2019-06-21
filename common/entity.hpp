#ifndef entity_hpp
#define entity_hpp

#include "hitbox.hpp"
#include "animation.hpp"
#include "coordinate.hpp"
#include "observer.hpp"
#include "serialize.hpp"
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
    using Type = std::string;

    struct {
        Signal<Entity &> im_dead;
    } signals;

    Coordinate coordinate;
    Animation animation;
    Hitbox hitbox;

    virtual ~Entity();
    Entity(const Type & id = "DEFAULT");

    void set_coordinate(const Coordinate & coord);
    void clear();
    const Type & get_type() const;
    Coordinate get_coordinate() const;
    Position get_position() const;
    bool operator==(const Entity & other) const;

    void serialize(IOWriter & out) const;
    std::string info() const;

private:
    Type type = "DEFAULT";
    float movement_cooldown = 0;
};

#endif
