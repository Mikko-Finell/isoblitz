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
    void center_window(int screen_w, int screen_h, int win_w, int win_h);

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);
};

#endif
