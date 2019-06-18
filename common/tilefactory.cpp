#include "tilefactory.hpp"
#include "database.hpp"
#include <iostream>

TileFactory::TileFactory(TileManager & tm, SpriteFactory & sf)
    : tilem(tm), spritef(sf)
{
    int origin_x, origin_y, w, h, columns, rows;
    const auto sqlquery = R"(
        SELECT Tile_Region.name, Entity.tileset_origin_x, Entity.tileset_origin_y, 
            Entity.sprite_w, Entity.sprite_h, 
            Tile_region.columns, Tile_region.rows
        FROM Tile_Region INNER JOIN Entity ON Tile_Region.name = Entity.name
    )";
    Database db{"TileFactory"};

    // TODO hard
    // Note that this assumes only 1 tile_region, and will have to be made
    // mode sophisticated if additional regions are required.
    db.execute(sqlquery, [&](sqlite3_stmt * stmt){
        int column = 0;

        // The region_name represents different tilesets or regions within a
        // tileset, eg floor, walls, and so on.
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

    Tile::ID next_id = 0;
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            int x = origin_x + column * w;
            int y = origin_y + row * h;
            Tile::ID id = ++next_id;

            tiles.emplace(id, Tile{id});
            auto & tile = tiles[id];

            auto pair = std::make_pair<>(id, SpriteImpl{});
            pair.second.set_texcoords({x, y, w, h})
                .set_position({0, 0})
                .set_size(w, h)
                .set_layer(config::tile_layer)
                .set_offset(w / 2, h / 2);
            sprites.emplace(pair);
        }
    }
}

Tile * TileFactory::create(RenderSystem & rs, Tile::ID id) const {
    Tile * tile = tilem.alloc();
    try {
        *tile = tiles.at(id);
    }
    catch (std::out_of_range) {
        tilem.destroy(tile);
        std::cerr << "\nERROR: TileFactory::create(" << id << ")\n" << std::endl;
        throw;
    }
    tile->sprite = spritef.create_from_impl(rs, &sprites.at(id));
    return tile;
}

std::vector<Tile::ID> TileFactory::get_all() const {
    std::vector<Tile::ID> v;
    v.reserve(tiles.size());
    for (auto & pair : tiles) {
        v.push_back(pair.second.get_id());
    }
    return v;
}
