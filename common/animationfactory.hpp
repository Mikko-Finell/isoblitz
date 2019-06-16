#ifndef animationfactory_hpp
#define animationfactory_hpp

#include "animation.hpp"
#include "animationmanager.hpp"
#include "spritefactory.hpp"
#include "rendersystem.hpp"
#include "util.hpp"
#include <sqlite3.h>
#include <unordered_map>
#include <string>

/**
 * AnimationFactory
 * Holds all available animations.
 */
class AnimationFactory {
    AnimationManager & animm;
    AnimationSystem & anims;
    SpriteFactory & spritef;
    std::unordered_map<std::string, Animation> animations;

public:
    AnimationFactory(AnimationManager & am, AnimationSystem & as, SpriteFactory & sf);

    // Create an animation with a sprite using rendersys parameter.
    Animation * create(RenderSystem & rs, const type_id_t & type) const;
};

#endif
