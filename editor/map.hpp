#ifndef __MAP__
#define __MAP__

#include "tile.hpp"
#include "common/serializable.hpp"
#include "common/observer.hpp"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

class Map : public Observer, public Serializable {
    const std::string extension = ".bulletmap";
    std::vector<Tile> tiles;
    gfx::Manager & spritem;

    void serialize(std::ostream & out) const override;
    void deserialize(std::istream & in) override;

public:
    struct {
        Signal<float,float> map_loaded;
    } signal;

    std::string name = "tmp";
    std::string filename() const {
        return "../maps/" + name + extension;
    }

    Map(gfx::Manager & sm);

    void undo();
    void create(const Tile & tile);
    void remove(const sf::Vector2f & coord); // TODO verify this wont ever fail
    void draw(VertexArray & vertices);

    void on_new(const std::string & s);
    void on_save(const std::string & s);
    void on_load(const std::string & s);
    void on_setname(const std::string & s);
};

#endif
