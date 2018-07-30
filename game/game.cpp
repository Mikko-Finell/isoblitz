#include <iostream>
#include "common/serializable.hpp"
#include "common/drawable.hpp"

class Map : public Serializable, public Drawable {
    void deserialize(std::istream & in) override {
    }

public:
    void draw(VertexArray & vertices) const override {
    }
};

int main() {
}
