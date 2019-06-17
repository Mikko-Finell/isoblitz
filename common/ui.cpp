#include "ui.hpp"

namespace UI {

Element::Element() {
}

Element::~Element() {
}

void Element::_activate() {
}

void Element::_hover() {
}

void Element::_init() {
}

void Element::activate() {
    _activate();
}

void Element::hover() {
    _hover();
}

void Element::init() {
    _init();
}

// Container /////////////////////////////////////////////////////////////////////////////

void Container::cleanup() {
    spritem.destroy(background);
    for (auto & element : elements) {
        spritem.destroy(element.sprite.idle);
        spritem.destroy(element.sprite.hovering);
        spritem.destroy(element.sprite.activated);
    }
    for (auto & element : element_ptrs) {
        spritem.destroy(element->sprite.idle);
        spritem.destroy(element->sprite.hovering);
        spritem.destroy(element->sprite.activated);
    }
}

Container::~Container() {
    cleanup();
}

Container::Container(input::Manager & inputm, SpriteManager & sm) :spritem(sm) {
}

void Container::add_element(Element * element) {
    element_ptrs.push_back(element);
}

} // UI
