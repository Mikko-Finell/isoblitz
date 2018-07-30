#ifndef __DRAWABLE__
#define __DRAWABLE__

#include <vector>
#include <SFML/Graphics/Vertex.hpp>

using VertexArray = std::vector<sf::Vertex>;

class Drawable {
public:
    virtual void draw(VertexArray & vertices) const = 0;
    virtual ~Drawable() {}
};

#endif
