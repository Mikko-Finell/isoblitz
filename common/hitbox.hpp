#ifndef hitbox_hpp
#define hitbox_hpp

#include "util.hpp"
#include <SFML/Graphics/Rect.hpp>

class Hitbox {
    sf::IntRect rect;

public:
    Hitbox();
    Hitbox(int offset_x, int offset_y, int w, int h);
    void set_position(cell_t);

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);
};

#endif
