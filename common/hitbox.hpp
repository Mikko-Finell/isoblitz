#ifndef hitbox_hpp
#define hitbox_hpp

#include "util.hpp"
#include "coordinate.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <sstream>

class Hitbox {
    sf::Vector2i offset;
    sf::IntRect screencoords;

public:
    Hitbox();
    Hitbox(int offset_x, int offset_y, int w, int h);
    void set_position(const Position & pos);

    operator sf::IntRect() const {
        return screencoords;
    }

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);

    std::string info() const {
        std::stringstream ss; ss << "Hitbox{"
            << " Offset(" << offset.x << "," << offset.y << ") ; Screencoords("
            << util::rect_to_str(screencoords) << ") }";
        return ss.str();
    }
};

#endif
