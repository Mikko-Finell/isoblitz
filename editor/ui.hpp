#ifndef __UI__
#define __UI__

#include <vector>
#include <SFML/Graphics.hpp>
#include "common/observer.hpp"
#include "common/position.hpp"

class UI {
    sf::RenderWindow * window;
    sf::Vector2f prev_mouse_pos;
    sf::Vector2f current_mouse_dt;
    bool mouse_pressed = false;

public:
    struct {
        Signal<> quit;
        Signal<const std::string &> setspritesheet;

        Signal<const std::string &> save;
        Signal<const std::string &> newmap;
        Signal<const std::string &> load;
        Signal<const std::string &> setmapname;

        Signal<> paint;
        Signal<> erase;
        Signal<const Position &> update_mousepos;
        Signal<bool> setblocked;
        Signal<const Coordinate &> setsprite;

        Signal<> undo;
    } signal;

    UI(sf::RenderWindow & w);

    bool is_mouse_pressed();
    Position mouse_pos();
    void process_input();
};

#endif
