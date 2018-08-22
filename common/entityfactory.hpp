#ifndef entityfactory_hpp
#define entityfactory_hpp

#include "entity.hpp"
#include "spritefactory.hpp"
#include "animationfactory.hpp"
#include "rendersystem.hpp"
#include "util.hpp"
#include <unordered_map>

class EntityFactory {
    mutable uid_t next_id = 0;
    std::unordered_map<type_id_t, Entity> entities;
    AnimationFactory & animf;
    RenderSystem & render;

public:
    EntityFactory(AnimationFactory & af, RenderSystem & rs);
    Entity get(const type_id_t & type) const;
    std::vector<Entity> get_all() const;
};

#endif
