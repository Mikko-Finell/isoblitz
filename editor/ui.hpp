#ifndef __UI__
#define __UI__

#include <vector>
#include <SFML/Graphics.hpp>
#include "position.hpp"
#include "event.hpp"
#include "emitter.hpp"

class UI : public Emitter {
    sf::RenderWindow * window;
    sf::Vector2f prev_mouse_pos;
    sf::Vector2f current_mouse_dt;
    bool mouse_pressed = false;

public:
    UI(sf::RenderWindow & w);

    bool is_mouse_pressed();
    sf::Vector2f mouse_dt();
    Position mouse_pos();
    std::vector<Event> handle_events();
};

#endif
