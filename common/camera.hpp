#ifndef camera_hpp
#define camera_hpp

#include <SFML/Graphics.hpp>

class Camera {
    sf::RenderWindow & window;
    float zoomfactor = 1.0f;

public:
    Camera(sf::RenderWindow & w);
    void zoom(float factor);
    void scroll(const sf::Vector2f & dt);
    void focus_at(const sf::Vector2f & point);
};

#endif
