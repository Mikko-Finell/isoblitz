#include "spritemanager.hpp"

Sprite * SpriteManager::alloc() {
    return new Sprite;
}

void SpriteManager::destroy(Sprite & sprite) {
    for (auto itr = sprites.begin(); itr != sprites.end(); itr++) {
        if (&(*itr) == &sprite) {
            sprites.erase(itr);
            return;
        }
    }
    throw std::out_of_range{"Attempt destroy non-existant sprite."};
}
