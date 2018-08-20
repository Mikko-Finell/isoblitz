#include "camera.hpp"
#include "util.hpp"

Camera::Camera(sf::RenderWindow & w) : window(w) {
}

void Camera::zoom(float factor) {
    zoomfactor /= factor;
    auto v = window.getView();
    v.zoom(1.0f / factor);
    window.setView(v);
}

void Camera::scroll(const sf::Vector2f & dt) {
    auto view = window.getView();
    view.move(zoomfactor * dt);
    window.setView(view);
}

void Camera::focus_at(const sf::Vector2f & point) {
    auto view = window.getView();
    view.setCenter(point);
    window.setView(view);
}

void Camera::center_window(int screen_w, int screen_h, int win_w, int win_h) {
    window.setPosition(sf::Vector2i{screen_w/2-win_w/2, screen_h/2-win_h/2});
}

void Camera::serialize(std::ostream & out) const {
    auto view = window.getView();
    auto center = view.getCenter();
    util::write(center.x, out);
    util::write(center.y, out);
    util::write(zoomfactor, out);
}

void Camera::deserialize(std::istream & in) {
    sf::Vector2f center;
    util::read(center.x, in);
    util::read(center.y, in);

    float factor;
    util::read(factor, in);
    
    window.setView(window.getDefaultView());
    focus_at(center);
    zoomfactor = 1.0f;
    zoom(1.0f / factor);
}
