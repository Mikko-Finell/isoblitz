#include "event.hpp"

Event::Event(Type type, void * d, void * s)
    : type(type), data(d), sender(s)
{}

/*Event::Event(const Event & other)
    : type(other.type), data(other.data), sender(other.sender)
{
    coordinate = other.coordinate;
}*/

bool Event::operator==(Type type) const {
    return this->type == type;
}
