#include "spritemanager.hpp"
#include <iostream>

SpriteManager::~SpriteManager() {
}

Sprite * SpriteManager::alloc() {
    sprites.emplace_back();
    //std::cout << "Create sprite at " << &sprites.back() << std::endl;
    return &sprites.back();
}

void SpriteManager::destroy(Sprite & sprite) {
    //assert(sprite.renderer != nullptr);
    if (sprite.renderer != nullptr) {
        sprite.renderer->remove(&sprite);
    }
    else {
        std::cout << "WARNING: destroy sprite with no renderer.\n";
    }
    for (auto itr = sprites.begin(); itr != sprites.end(); itr++) {
        if (&(*itr) == &sprite) {
            //std::cout << "Erase sprite at " << &sprite << "\n";
            sprites.erase(itr);
            //std::cout << sprites.size() << " sprites remain.\n";
            return;
        }
    }
    throw std::out_of_range{"Attempt destroy non-existant sprite."};
}
