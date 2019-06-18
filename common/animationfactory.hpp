#ifndef animationfactory_hpp
#define animationfactory_hpp

#include "animation.hpp"
#include "spritefactory.hpp"
#include "rendersystem.hpp"
#include "entity.hpp"
#include <sqlite3.h>
#include <unordered_map>
#include <string>

/**
 * AnimationFactory
 * Holds all available animations.
 */
class AnimationFactory {
    AnimationSystem & anims;
    SpriteFactory & spritef;
    std::unordered_map<std::string, Animation> animations;

public:
    AnimationFactory(AnimationSystem & as, SpriteFactory & sf);

    // Create an animation with a sprite using rendersys parameter.
    Animation create(RenderSystem & rs, const std::string & name) const;
};

#endif
