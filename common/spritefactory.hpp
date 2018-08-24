#ifndef spritefactory_hpp
#define spritefactory_hpp

#include "sprite.hpp"
#include <string>
#include <unordered_map>

class SpriteFactory {
    using key_t = std::string;
    std::unordered_map<key_t, std::unordered_map<key_t, Sprite>> gomap;

public:
    SpriteFactory();
    Sprite get(const std::string & entity, const std::string & sprite);
};

#endif
