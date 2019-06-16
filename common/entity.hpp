#ifndef entity_hpp
#define entity_hpp

#include "hitbox.hpp"
#include "animation.hpp"
#include "animationfactory.hpp"
#include "coordinate.hpp"
#include "observer.hpp"
#include "util.hpp"
#include <sstream>
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
    type_id_t type_id;
    uid_t _uid;
    float movement_cooldown = 0;

public:
    struct {
        Signal<const Position &> position;
        Signal<const cell_t &> coordinate;
    } signal;

    std::list<cell_t> path;

    cell_t cell;
    cell_t target;

    EntitySystem * entitys = nullptr;
    Animation * animation = nullptr;
    Hitbox hitbox;

    Entity(const uid_t & id = 0, const type_id_t & type = "DEFAULT");
    virtual ~Entity();

    void update(time_t dt);
    void set_cell(const cell_t & c);
    void set_hitbox(const Hitbox & hb);

    inline cell_t get_cell() const {
        return cell;
    }

    inline type_id_t get_type() const {
        return type_id;
    }

    const uid_t uid() const {
        return _uid;
    }

    const uid_t uid(const uid_t & id) {
        _uid = id;
        return uid();
    }

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);

    bool operator==(const Entity & other) const {
        return uid() == other.uid();
    }

    std::string info() const {
        std::stringstream ss; ss << "Entity, id=" << uid()
            << ", type=" << type_id << "\n\t" << cell.info()
            << ", " << cell.to_pixel().info() << std::endl;
        ss << "\t" << hitbox.info() << std::endl;
        return ss.str();
    }
};

/**
 * EntitySystem
 * Responsible to updating entities.
 */
class EntitySystem {
    std::unordered_set<Entity *> entities;

    // TODO hard critical
    // Solve the spawn/erase-while-updating problem

public:
    virtual ~EntitySystem() {
    }

    void add(Entity * entity);
    inline void add(Entity & entity) {
        add(&entity);
    }
    void remove(Entity & entity);
    inline void remove(Entity * entity) {
        remove(*entity);
    }

    void update(time_t dt);
};

#endif
