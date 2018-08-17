#include "camera.hpp"

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
