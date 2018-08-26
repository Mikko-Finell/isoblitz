#ifndef sprite_hpp
#define sprite_hpp

#include "rendersystem.hpp"
#include "util.hpp"
#include <SFML/Graphics.hpp>
#include <sstream>

class Sprite : public GameObject {
    sf::FloatRect screencoords;
    sf::IntRect spritecoords;
    sf::Vector2i offset;
    int layer = 0;
    bool visible = true;

public:
    ~Sprite();

    Sprite();
    Sprite(RenderSystem & rs);
    Sprite(const Sprite & other);
    Sprite & operator=(const Sprite & other);

    void draw(sf::Vertex * vs) const;
    Sprite & show();
    Sprite & hide();

    Sprite & set_spritecoords(const sf::IntRect & coords);
    Sprite & set_screencoords(const sf::FloatRect & coords);
    Sprite & set_position(float x, float y);
    Sprite & set_position(const sf::Vector2f & v);
    Sprite & set_size(int w, int h);
    Sprite & set_offset(int x, int y);
    Sprite & set_layer(int z);

    bool operator>(const Sprite & other) const;
    inline bool operator<(const Sprite & other) const {
        return !(operator>(other));
    }

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);

    inline int get_layer() const {
        return layer;
    }

    inline const sf::FloatRect & get_screencoords() const {
        return screencoords;
    }

    inline Position get_position() const {
        return Position{screencoords.left, screencoords.top};
    }

    inline Position get_origin() const {
        return get_position() + offset;
    }

    std::string info() const {
        std::stringstream ss; ss << "Sprite:\n"
            << "\tScreencoords{" << util::rect_to_str(screencoords) << "}\n"
            << "\tSpritecoords{" << util::rect_to_str(spritecoords) << "}\n"
            << "\tOffset{" << util::vec_to_str(offset) << "}\n"
            << "\tLayer = " << layer << "\n"
            << std::boolalpha << "\tVisible = " << visible << std::endl;
        return ss.str();
    }
};

#endif
