#ifndef entity_hpp
#define entity_hpp

#include "hitbox.hpp"
#include "animation.hpp"
#include "animationfactory.hpp"
#include "coordinate.hpp"
#include "util.hpp"

class Entity {
    type_id_t type_id;
    uid_t id;

    //Entity & operator=(const Entity & other) = delete;

public:
    cell_t cell;
    Animation animation;
    Hitbox hitbox;

    Entity(const type_id_t & type = "DEFAULT");
    Entity(std::istream & in, AnimationFactory & animf);

    void init(uid_t id, AnimationFactory & animf);
    void update(time_t dt);
    void destroy();

    void set_cell(const cell_t & c);
    void set_hitbox(const Hitbox & hb);

    inline cell_t get_cell() const {
        return cell;
    }

    inline type_id_t get_type() const {
        return type_id;
    }

    inline uuid_t get_uid() const {
        return id;
    }

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in, AnimationFactory & animf);
};

#endif
