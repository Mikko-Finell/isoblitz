#include "coordinate.hpp"
#include "util.hpp"

void Position::serialize(std::ostream & out) const {
    util::write(x, out);
    util::write(y, out);
}

void Position::deserialize(std::istream & in) {
    util::read(x, in);
    util::read(y, in);
}

template<int W, int H>
void Coordinate<W, H>::serialize(std::ostream & out) const {
    util::write(x, out);
    util::write(y, out);
}

template<int W, int H>
void Coordinate<W, H>::deserialize(std::istream & in) {
    util::read(x, in);
    util::read(y, in);
}

template class Coordinate<CELLW, CELLH>;
template class Coordinate<TILEW, TILEH>;
