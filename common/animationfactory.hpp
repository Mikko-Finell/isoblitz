#ifndef animationfactory_hpp
#define animationfactory_hpp

#include "animation.hpp"
#include "rendersystem.hpp"
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
