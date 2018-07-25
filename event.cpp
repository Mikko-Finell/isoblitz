#include "event.hpp"

Event::Event(Type type, void * d, void * s)
    : type(type), data(d), sender(s)
{}

bool Event::operator==(Type type) const {
    return this->type == type;
}
