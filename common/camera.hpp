#ifndef camera_hpp
#define camera_hpp

#include "serialize.hpp"
#include <SFML/Graphics.hpp>

/** 
 * Camera
 * Interface to control viewing the world.
 */
class Camera {
    sf::RenderWindow & window;
    float zoomfactor = 1.0f;

public:
    Camera(sf::RenderWindow & w);

    // if factor>1 the world is zoomed in
    void zoom(float factor);

    // move the camera in the dt direction
    void scroll(const sf::Vector2f & dt);

    // centers the view at the point
    void focus_at(const sf::Vector2f & point);

    // centers the actual window using the params
    void center_window(int screen_w, int screen_h, int win_w, int win_h);

    void serialize(IOWriter & out);
    void deserialize(IOReader & in);
};

#endif
