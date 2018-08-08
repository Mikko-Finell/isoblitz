#ifndef __SERIAL__
#define __SERIAL__

#include <fstream>

template<typename T>
inline void write(const T & t, std::ostream & out) {
    out.write(reinterpret_cast<const char*>(&t), sizeof(t));
}

template<typename T>
inline void read(T & t, std::istream & in) {
    in.read(reinterpret_cast<char*>(&t), sizeof(t));
}

class Serializable {
    friend std::ostream & operator<<(std::ostream &out, const Serializable & obj);
    friend std::istream & operator>>(std::istream &out, Serializable & obj);
    virtual void serialize(std::ostream & out) const = 0;
    virtual void deserialize(std::istream & in) = 0;

protected:
    virtual ~Serializable() {}
};

inline std::ostream & operator<<(std::ostream & out, const Serializable & obj) {
    obj.serialize(out);
    return out;
}

inline std::istream & operator>>(std::istream & in, Serializable & obj) {
    obj.deserialize(in);
    return in;
}

#endif
