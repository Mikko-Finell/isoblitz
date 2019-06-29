#ifndef animationfactory_hpp
#define animationfactory_hpp

#include "animation.hpp"
#include "spritefactory.hpp"
#include "rendersystem.hpp"
#include "entity.hpp"
#include "stl.hpp"

class AnimationFactory;
class ActionToDirectionMap {
    friend AnimationFactory;
    template<class T, class U> using map = std::unordered_map<T, U>;
    using DirectionSequenceMap = map<directions::Type, TexCoordSequence>;
    map<actions::Type, DirectionSequenceMap> action_to_dirmap;

    DirectionSequenceMap & operator[](const actions::Type & action);

public:
    TexCoordSequence * get(const actions::Type & action, const directions::Type & dir);
};

/**
 * AnimationFactory
 * Holds all available animations.
 */
class AnimationFactory {
    AnimationSystem & anims;
    SpriteFactory & spritef;
    RenderSystem & default_rs;

    mutable std::unordered_map<EntityType, ActionToDirectionMap> entity_to_actionmap;

public:
    AnimationFactory(AnimationSystem & as, SpriteFactory & sf, RenderSystem & rs);

    // Create an animation with a sprite using rendersys parameter.
    Animation create(RenderSystem & rs, const std::string & name) const;
    Animation create(const std::string & name) const;
};

#endif
