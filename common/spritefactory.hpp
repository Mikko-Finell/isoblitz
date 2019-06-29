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
    std::unordered_map<std::string, SpriteImpl> sprites;
    SpriteManager & spritem;
    RenderSystem & default_rs;

public:
    SpriteFactory(SpriteManager & sm, RenderSystem & rs);
    Sprite copy(RenderSystem & rs, const Sprite & source);
    SpriteImpl * copy(RenderSystem & rs, const SpriteImpl * source);
    Sprite create(RenderSystem & rs, const std::string & sprite_name);
    Sprite create(const std::string & sprite_name);

    SpriteImpl create_impl(const std::string & sprite_name);
    Sprite create_from_impl(RenderSystem & rs, const SpriteImpl * source);
};

#endif
