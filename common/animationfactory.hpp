#ifndef __animationfactory_hpp__
#define __animationfactory_hpp__

#include "animation.hpp"
#include "rendersystem.hpp"
#include <sqlite3.h>
#include <unordered_map>
#include <string>

class AnimationFactory {
    std::unordered_map<std::string, Animation> animations;
    RenderSystem & render;

public:
    AnimationFactory(RenderSystem & rs);
    Animation get(const std::string & name);
};

#endif
