#include "tilefactory.hpp"
#include "database.hpp"
#include <iostream>

TileFactory::TileFactory(RenderSystem & rs) : render(rs) {
    int origin_x, origin_y, w, h, columns, rows;
    const auto sqlquery = R"(
        SELECT name, sprite_origin_x, sprite_origin_y, sprite_w, sprite_h, 
            columns, rows
        FROM Tile_Region
    )";
    Database db{"TileFactory"};
    db.execute(sqlquery, [&](sqlite3_stmt * stmt){
        int column = 0;

        // TODO easy
        // find out what this is
        std::string region_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };

        origin_x = sqlite3_column_int(stmt, column++);
        origin_y = sqlite3_column_int(stmt, column++);
        w = sqlite3_column_int(stmt, column++);
        h = sqlite3_column_int(stmt, column++);
        columns = sqlite3_column_int(stmt, column++);
        rows = sqlite3_column_int(stmt, column++);
    });

    tile_id_t next_id = 0;
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            int x = origin_x + column * w;
            int y = origin_y + row * h;
            auto id = ++next_id;

            tiles.emplace(id, Tile{id});
            auto & tile = tiles[id];

            // TODO easy critical
            // tmp workaround for debug purposes
            tile.sprite = new Sprite;
            tile.sprite->set_spritecoords({x, y, w, h})
                .set_position({0, 0})
                .set_size(w, h)
                .set_layer(TILE_LAYER)
                .set_offset(w / 2, h / 2);
        }
    }
}

Tile TileFactory::get(tile_id_t id) const {
    // TODO critical
    // adding the sprite of a tmp object thats destroyed 
    // upon exiting this method scope.
    throw std::logic_error{"tilefactory get not implemented"};
    auto tile = tiles.at(id);
    render.add(tile.sprite, "TileFactory::get");
    return tile;
}

std::vector<Tile> TileFactory::get_all() const {
    std::vector<Tile> tilev;
    tilev.reserve(tiles.size());
    for (auto & pair : tiles) {
        tilev.push_back(pair.second);
    }
    return tilev;
}
