#ifndef entityfactory_hpp
#define entityfactory_hpp

#include "entity.hpp"
#include "spritefactory.hpp"
#include "animationfactory.hpp"
#include "rendersystem.hpp"
#include <unordered_map>

class EntityFactory {
    std::unordered_map<std::string, Entity> entities;
    AnimationFactory & animf;
    RenderSystem & render;

public:
    EntityFactory(AnimationFactory & af, RenderSystem & rs);
    Entity get(const std::string & name);
};

#endif
