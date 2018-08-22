#include "tile.hpp"

Tile::Tile(tile_id_t i) : id(i) {
}

Tile::Tile(std::istream & in, RenderSystem & rs) {
    deserialize(in);
    init(rs);
}

void Tile::init(RenderSystem & rs) {
    sprite.init(rs);
}

void Tile::set_spritedata(const SpriteData & spritedata) {
    sprite.set_data(spritedata);
}

void Tile::set_coordinate(int x, int y) {
    set_coordinate(coord_t{x, y});
}

void Tile::set_coordinate(const coord_t & c) {
    Position pos = c.to_pixel();
    sprite.set_position(pos);
    this->coord = c;
}

sf::Vector2f Tile::get_position() const {
    return coord.to_pixel();
}

coord_t Tile::get_coordinate() const {
    return coord;
}

tile_id_t Tile::get_id() const {
    return id;
}

SpriteData Tile::get_spritedata() const {
    return sprite.get_spritedata();
}

void Tile::serialize(std::ostream & out) const {
    util::write(coord.x, out);
    util::write(coord.y, out);
    util::write(id, out);
    util::write(layer, out);
    sprite.serialize(out);
}

void Tile::deserialize(std::istream & in) {
    util::read(coord.x, in);
    util::read(coord.y, in);
    util::read(id, in);
    util::read(layer, in);
    sprite.deserialize(in);
}