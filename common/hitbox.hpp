#ifndef hitbox_hpp
#define hitbox_hpp

#include "util.hpp"
#include "coordinate.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <sstream>

/**
 * Hitbox
 * Represents the clickable area of some gameobject.
 */
class Hitbox {
    // the distance from the upper-left corner of the
    // hitbox actual visual coordinates to the visual
    // center of the owning game object
    sf::Vector2i offset;

    sf::FloatRect screencoords;

public:
    Hitbox();
    Hitbox(int offset_x, int offset_y, int w, int h);
    void set_position(const Position & pos);

    inline Position get_position() const {
        return {screencoords.left + offset.x, screencoords.top + offset.y };
    }

    operator sf::FloatRect() const {
        return screencoords;
    }

    operator Position::Region () const {
        return Position::Region{screencoords};
    }

    std::string info() const {
        std::stringstream ss; ss << "Hitbox{"
            << " Offset(" << offset.x << "," << offset.y << ") ; Screencoords("
            << util::rect_to_str(screencoords) << ") }";
        return ss.str();
    }
};

#endif
