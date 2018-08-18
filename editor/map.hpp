#ifndef map_bpp
#define map_bpp

#include "common/rendersystem.hpp"
#include "common/observer.hpp"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

struct MapInfo {
    int columns = 0, rows = 0;
    int tilecount = 0;
    std::string name;
};

class Map {
    const std::string extension = ".bulletmap";
    const std::string mapdir = "../maps/";

    MapInfo mapinfo;

    RenderSystem & render;

public:
    struct {
        Signal<float,float> map_loaded;
    } signal;

    inline std::string filename() const {
        return mapdir + mapinfo.name + extension;
    }

    Map(RenderSystem & rs);

    void load(const std::string & mapname);
};

#endif
