#include "serialize.hpp"

IOWriter::IOWriter(const std::string & filename) {
    if (out = std::ofstream{filename, std::ios::binary}; out.fail()) {
        throw std::invalid_argument{"Couldn't open " + filename};
    }
}

IOReader::IOReader(const std::string & filename) {
    if (in = std::ifstream{filename, std::ios::binary}; in.fail()) {
        throw std::invalid_argument{"Couldn't open " + filename};
    }
}

template<>
void IOWriter::write<std::string>(const std::string & str) {
    write(str.length());
    out << str;
    assert(out.good());
}

template<>
void IOReader::read<std::string>(std::string & str) {
    assert(str.empty());
    std::string::size_type length;
    read(length);
    str.resize(length);
    in.read(str.data(), length);
    assert(in.good());
}

template<>
void IOWriter::write<Coordinate>(const Coordinate & coord) {
    write(coord.x);
    write(coord.y);
}

template<>
void IOReader::read<Coordinate>(Coordinate & coord) {
    read(coord.x);
    read(coord.y);
}
