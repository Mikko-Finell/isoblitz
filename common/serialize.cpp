#include "serialize.hpp"

void IOWriter::check_ok() {
    if (out.fail()) {
        std::cerr << "IOWriter error: " << std::strerror(errno) << std::endl;
        if (out.bad()) {
            std::terminate();
        }
    }
}

IOWriter::IOWriter(const std::string & filename) {
    out = std::ofstream{filename, std::ios::binary};
    if (out.is_open() == false) {
        throw std::invalid_argument{"IOWriter couldn't open " + filename};
    }
}

void IOReader::check_ok() {
    if (in.fail()) {
        std::cerr << "IOReader error: " << std::strerror(errno) << std::endl;
        if (in.bad()) {
            std::terminate();
        }
    }
}

IOReader::IOReader(const std::string & filename) {
    in = std::ifstream{filename, std::ios::binary}; 
    if (in.is_open() == false) {
        throw std::invalid_argument{"IOReader couldn't open " + filename};
    }
}

template<>
void IOWriter::write<std::string>(const std::string & str) {
    write(str.length());
    out << str;
    check_ok();
}

template<>
void IOReader::read<std::string>(std::string & str) {
    assert(str.empty());
    std::string::size_type length;
    read(length);
    str.resize(length);
    in.read(str.data(), length);
    check_ok();
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
