#ifndef serialize_hpp
#define serialize_hpp

#include "coordinate.hpp"
#include <fstream>
#include <cassert>

class IOWriter {
    std::ofstream out;

public:
    IOWriter(const std::string & filename);

    template<class T>
    void write(const T & t) {
        out.write(reinterpret_cast<const char*>(&t), sizeof(t));
        assert(out.good());
    }
    
};

class IOReader {
    std::ifstream in;

public:
    IOReader(const std::string & filename);

    template<typename T>
    void read(T & t) {
        in.read(reinterpret_cast<char*>(&t), sizeof(t));
        assert(in.good());
    }
};

#endif
