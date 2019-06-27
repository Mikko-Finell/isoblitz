#ifndef spritefactory_hpp
#define spritefactory_hpp

#include "sprite.hpp"
#include "spritemanager.hpp"
#include <string>
#include <unordered_map>
#include <memory>

/**
 * SpriteFactory
 * Knows about sprites by name.
 */
class SpriteFactory {
public:
    using KeyType = std::string;

    SpriteFactory(SpriteManager & sm, RenderSystem & rs);
    Sprite copy(RenderSystem & rs, const Sprite & source);
    SpriteImpl * copy(RenderSystem & rs, const SpriteImpl * source);
    Sprite create(RenderSystem & rs, const KeyType & sprite_name);
    Sprite create(const KeyType & sprite_name);

    SpriteImpl create_impl(const KeyType & sprite_name);
    Sprite create_from_impl(RenderSystem & rs, const SpriteImpl * source);

private:
    std::unordered_map<KeyType, SpriteImpl> sprites;
    SpriteManager & spritem;
    RenderSystem & default_rs;
};

#endif
