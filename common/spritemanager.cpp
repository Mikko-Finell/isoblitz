#include "spritemanager.hpp"
#include <iostream>

SpriteManager::~SpriteManager() {
}

Sprite * SpriteManager::alloc() {
    sprites.emplace_back();
    //std::cout << "Create sprite at " << &sprites.back() << std::endl;
    return &sprites.back();
}

void SpriteManager::destroy(Sprite * sprite) {
    if (sprite->renderer != nullptr) {
        sprite->renderer->remove(sprite);
    }
    else {
        std::cout << "WARNING: destroy sprite with no renderer.\n";
    }
    for (auto itr = sprites.begin(); itr != sprites.end(); itr++) {
        if (&(*itr) == sprite) {
            sprites.erase(itr);
            return;
        }
    }
    throw std::logic_error{"Attempt destroy non-existant sprite."};
}
