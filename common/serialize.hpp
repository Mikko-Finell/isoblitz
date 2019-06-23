#ifndef serialize_hpp
#define serialize_hpp

#include "coordinate.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <cassert>

class IOWriter {
    std::ofstream out;

    void check_ok();

public:
    IOWriter(const std::string & filename);

    template<class T>
    void write(const T & t) {
        out.write(reinterpret_cast<const char*>(&t), sizeof(t));
        check_ok();
    }
};

class IOReader {
    std::ifstream in;

    void check_ok();

public:
    IOReader(const std::string & filename);

    template<typename T>
    void read(T & t) {
        in.read(reinterpret_cast<char*>(&t), sizeof(t));
        check_ok();
    }
};

#endif
