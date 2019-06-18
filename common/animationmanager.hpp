#ifndef animationmanager_hpp
#define animationmanager_hpp

#include "animation.hpp"
#include <list>

class AnimationManager {
    std::list<Animation> animations;

public:
    AnimationManager();
    Animation * alloc();
    void destroy(Animation * anim);
};

#endif
