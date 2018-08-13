#ifndef __UI__
#define __UI__

#include "common/observer.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

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
        Signal<> toggle_snap;
        Signal<const sf::Vector2f &> update_mousepos;
        Signal<bool> setblocked;
        Signal<const sf::Vector2i &> setsprite;

        Signal<> undo;
    } signal;

    UI(sf::RenderWindow & w);

    bool is_mouse_pressed();
    sf::Vector2f mouse_pos();
    void process_input();
};

#endif
