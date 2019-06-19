#ifndef tilemenu_hpp
#define tilemenu_hpp

#include "common/ui.hpp"
#include "common/engine.hpp"

class TileMenuItem : public UI::Element {
public:
    void activate() override;
    void init() override;
    void update_mousepos(const Position & pos) override;
    bool contains(const Position & pos) override;
    void click(const Position & pos) override;

    sf::FloatRect rect;
};

class TileMenu : public UI::Container {
    void cleanup() override;

public:
    Signal<Tile::ID> tile_selected;

    TileMenu(Engine & engine);
    void update_mousepos(const Position & pos) override;
};

#endif
