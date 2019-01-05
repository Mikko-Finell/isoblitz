#ifndef animationfactory_hpp
#define animationfactory_hpp

#include "animation.hpp"
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
    AnimationSystem & anims;
    std::unordered_map<std::string, Animation> animations;

public:
    AnimationFactory(AnimationSystem & as);

    // Retrieve the animation beloning to type.
    // The resulting animation is ready to play.
    Animation get(const type_id_t & type) const;
};

#endif
