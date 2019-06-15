#ifndef entitymenu_hpp
#define entitymenu_hpp

#include "common/entityfactory.hpp"
#include "common/observer.hpp"
#include <list>

class EntityMenuItem {
    Entity * entity = nullptr;
    sf::FloatRect rect;

    struct {
        Sprite hovering;
        Sprite selected;
    } sprite;

public:
    Signal<type_id_t> clicked;

    ~EntityMenuItem();
    EntityMenuItem(Entity * e);

    void init(SpriteFactory & sf, RenderSystem & rs);
    void set_screencoords(const sf::FloatRect & rect);
    void update_mousepos(const Position & p);
    bool try_click(const Position & p);
};

class EntityMenu : public Observer {
    std::list<EntityMenuItem> buttons;
    Sprite background;

    Position origin;
    int width, height;
    int columns;

public:
    Signal<type_id_t> entity_selected;

    inline void set_origin(const Position & p) { origin = p; }
    EntityMenu(SpriteFactory & sf, RenderSystem & rs, EntityFactory & ef,
            int w, int h, int c);

    void update_mousepos(const Position & p);
    bool try_click(const Position & p);
    inline bool contains(const Position & p) const {
        return  sf::IntRect(origin.x, origin.y, width, height).contains(p);
    }
};

#endif
