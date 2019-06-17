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
    engine.spritem.destroy(background);
    for (auto & element : elements) {
        engine.spritem.destroy(element.sprite.idle);
        engine.spritem.destroy(element.sprite.hovering);
        engine.spritem.destroy(element.sprite.activated);
    }
    for (auto & element : element_ptrs) {
        engine.spritem.destroy(element->sprite.idle);
        engine.spritem.destroy(element->sprite.hovering);
        engine.spritem.destroy(element->sprite.activated);
    }
}

Container::~Container() {
    cleanup();
}

Container::Container(Engine & eng) : engine(eng) {
}

void Container::add_element(Element * element) {
    element_ptrs.push_back(element);
}

} // UI
