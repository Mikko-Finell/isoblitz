#include "animationmanager.hpp"
#include <cassert>

AnimationManager::AnimationManager(SpriteManager & sm) : spritem(sm) {
}

Animation * AnimationManager::alloc() {
    animations.emplace_back("DEFAULT");
    return &animations.back();
}

void AnimationManager::destroy(Animation * anim) {
    for (auto itr = animations.begin(); itr != animations.end(); itr++) {
        if (&(*itr) == anim) {
            assert(anim->sprite != nullptr);
            spritem.destroy(anim->sprite);
            animations.erase(itr);
            return;
        }
    }
    throw std::out_of_range{"Attempt destroy non-existant animation."};
}

