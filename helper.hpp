#ifndef __HELPER__
#define __HELPER__

#define WINW 800
#define WINH 600
#define TILEW 128.0f
#define TILEH 64.0f
#define HALFW (TILEW / 2)
#define HALFH (TILEH / 2)

inline sf::Vector2f logic_to_pixel(const sf::Vector2f & v) {
    float x = (v.x - v.y) * HALFW;
    float y = (v.x + v.y) * HALFH;
    return {x, y};
}

inline sf::Vector2f pixel_to_logic(const sf::Vector2f & v) {
    float x = (v.x / HALFW + v.y / HALFH) / 2;
    float y = (v.y / HALFH - v.x / HALFW) / 2;
    return {x, y};
}

inline sf::Vector2f snap_to_grid(const sf::Vector2f & v) {
    auto w = pixel_to_logic(v);
    w.x = floor(w.x);
    w.y = floor(w.y);
    return logic_to_pixel(w);
}

inline sf::Vector2f tile_center_at(sf::Vector2f v) {
    v.x -= HALFW;
    //v.y -= HALFH;
    auto w = snap_to_grid(v);
    return pixel_to_logic(w);
}
/*
void user_click(sf::Vector2f v, std::vector<Tile> & tiles) {
    tiles.push_back(Tile{tile_center_at(v)});
}
*/

#endif
