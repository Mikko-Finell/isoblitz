#ifndef __UI__
#define __UI__

#include <vector>
#include <SFML/Graphics.hpp>
#include "common/observer.hpp"
#include "common/observer.hpp"
#include "common/position.hpp"

class UI {
    sf::RenderWindow * window;
    sf::Vector2f prev_mouse_pos;
    sf::Vector2f current_mouse_dt;
    bool mouse_pressed = false;

public:
    struct {
        Event<> quit;
        Event<const std::string &> setspritesheet;

        Event<const std::string &> save;
        Event<const std::string &> newmap;
        Event<const std::string &> load;
        Event<const std::string &> setmapname;

        Event<> paint;
        Event<> erase;
        Event<const Position &> update_mousepos;
        Event<bool> setblocked;
        Event<const Coordinate &> setsprite;

        Event<> undo;
    } events;

    UI(sf::RenderWindow & w);

    bool is_mouse_pressed();
    Position mouse_pos();
    void process_input();
};

#endif
