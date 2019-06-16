#ifndef tilemenu_hpp
#define tilemenu_hpp

#include "common/tilefactory.hpp"
#include "common/spritefactory.hpp"
#include "common/observer.hpp"
#include "common/sprite.hpp"
#include <list>

class TileMenuItem {
    Tile tile;
    sf::FloatRect rect;

    struct {
        Sprite * tile;
        Sprite * hovering;
        Sprite * selected;
    } sprite;

public:
    Signal<Tile::ID> clicked;

    TileMenuItem(const Tile & tile);
    void init(SpriteFactory & sf, RenderSystem & rs);
    void set_screencoords(const sf::FloatRect & rect);
    void update_mousepos(const Position & p);
    bool try_click(const Position & p);
};

class TileMenu : public Observer {
    std::list<TileMenuItem> buttons;
    Sprite * background;

    Position origin;
    int width, height;
    int columns;

public:
    Signal<Tile::ID> tile_selected;

    inline void set_origin(const Position & p) { origin = p; }
    TileMenu(SpriteFactory & sf, RenderSystem & rs, TileFactory & tf,
            int w, int h, int c);
    void update_mousepos(const Position & p);
    bool try_click(const Position & p);
    inline bool contains(const Position & p) const {
        return  sf::IntRect(origin.x, origin.y, width, height).contains(p);
    }
};

#endif
