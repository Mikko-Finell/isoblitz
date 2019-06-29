#include "spritemanager.hpp"
#include "stl.hpp"

SpriteManager::~SpriteManager() {
}

SpriteImpl * SpriteManager::alloc() {
    sprites.emplace_back();
    return &sprites.back();
}

void SpriteManager::destroy(SpriteImpl * sprite) {
    if (sprite == nullptr) {
        std::cout << "WARNING: attempt destroy nullptr sprite.\n";
    }
    else { 
        for (auto itr = sprites.begin(); itr != sprites.end(); itr++) {
            if (&(*itr) == sprite) {
                sprites.erase(itr);
                return;
            }
        }
        throw std::logic_error{"Attempt destroy non-existant sprite."};
    }
}
