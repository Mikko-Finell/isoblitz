#ifndef animationmanager_hpp
#define animationmanager_hpp

#include "animation.hpp"
#include "spritemanager.hpp"
#include <list>

class AnimationManager {
    SpriteManager & spritem;
    std::list<Animation> animations;

public:
    AnimationManager(SpriteManager & sm);
    Animation * alloc();
    void destroy(Animation * anim);
};

#endif
