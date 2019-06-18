#ifndef entity_hpp
#define entity_hpp

#include "hitbox.hpp"
#include "animation.hpp"
#include "coordinate.hpp"
#include "observer.hpp"
#include "util.hpp"
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
class EntitySystem;
class Entity {
public:
    using Name = std::string;
    using UID = std::size_t;

    struct {
        Signal<const Position &> position;
        Signal<const Cell &> coordinate;
    } signal;

    std::list<Cell> path;

    Cell cell;
    Cell target;

    EntitySystem * entitys = nullptr;
    Animation animation;
    Hitbox hitbox;

    Entity(const UID & id = 0, const Name & name = "DEFAULT");
    virtual ~Entity();

    void update(time_t dt);
    void set_cell(const Cell & c);
    void set_hitbox(const Hitbox & hb);

    Cell get_cell() const;
    Name name() const;
    const UID uid() const;
    const UID uid(const UID & id);

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);

    bool operator==(const Entity & other) const;

    std::string info() const;

private:
    Name _name;
    UID _uid;
    float movement_cooldown = 0;
};

/**
 * EntitySystem
 * Responsible to updating entities.
 */
class EntitySystem {
    std::unordered_set<Entity *> entities;

public:
    virtual ~EntitySystem() {
    }

    void add(Entity * entity, const std::string & who);
    inline void add(Entity & entity, const std::string & who) {
        add(&entity, who);
    }
    void remove(Entity * entity, const std::string & who);
    void update(time_t dt);
};

#endif
