#ifndef hitbox_hpp
#define hitbox_hpp

#include <SFML/Graphics/Rect.hpp>
using cell_t = sf::Vector2f;

class Hitbox {
    sf::IntRect rect;

public:
    Hitbox();
    Hitbox(int offset_x, int offset_y, int w, int h);
    void set_position(cell_t);
};

#endif
