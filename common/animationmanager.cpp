#include "animationmanager.hpp"
#include <cassert>

AnimationManager::AnimationManager() {
}

Animation * AnimationManager::alloc() {
    animations.emplace_back("DEFAULT");
    return &animations.back();
}

void AnimationManager::destroy(Animation * anim) {
    for (auto itr = animations.begin(); itr != animations.end(); itr++) {
        if (&(*itr) == anim) {
            animations.erase(itr);
            return;
        }
    }
    throw std::out_of_range{"Attempt destroy non-existant animation."};
}

