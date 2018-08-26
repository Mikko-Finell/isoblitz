#ifndef entity_hpp
#define entity_hpp

#include "hitbox.hpp"
#include "animation.hpp"
#include "animationfactory.hpp"
#include "coordinate.hpp"
#include "system.hpp"
#include "util.hpp"
#include <sstream>
#include <list>

class Entity : public GameObject {
    type_id_t type_id;
    uid_t uid;
    float movement_cooldown = 0;

public:
    std::list<cell_t> path;

    cell_t cell;
    cell_t target;

    Animation animation;
    Hitbox hitbox;

    Entity(const uid_t & id = 0, const type_id_t & type = "DEFAULT");

    void update(time_t dt);
    void set_cell(const cell_t & c);
    void set_hitbox(const Hitbox & hb);

    inline cell_t get_cell() const {
        return cell;
    }

    inline type_id_t get_type() const {
        return type_id;
    }

    inline void set_uid(const uid_t & id) {
        assert(uid == 0);
        uid = id;
    }

    inline uuid_t get_uid() const {
        return uid;
    }

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);

    std::string info() const {
        std::stringstream ss; ss << "Entity, id=" << uid
            << ", type=" << type_id << "\n\t" << cell.info()
            << ", " << cell.to_pixel().info() << std::endl;
        ss << "\t" << hitbox.info() << std::endl;
        return ss.str();
    }
};

class EntitySystem : public System {
    std::unordered_set<Entity *> entities;

    // TODO
    // Solve the spawn/erase-while-updating problem

public:
    void remove(GameObject * go) override;

    void remove(Entity * entity);
    inline void remove(Entity & entity) {
        remove(&entity);
    }

    void add(Entity * entity);
    inline void add(Entity & entity) {
        add(&entity);
    }

    void update(time_t dt);
};

#endif
