#include "tilefactory.hpp"
#include "database.hpp"
#include "stl.hpp"

TileFactory::TileFactory(SpriteFactory & sf, RenderSystem & rs)
    : spritef(sf), default_rs(rs)
{
    int origin_x, origin_y, sprite_w, sprite_h, columns, rows;
    const auto sqlquery = R"(
        SELECT Tile_Region.name, 
            Sprite.x, Sprite.y, Sprite.w, Sprite.h,
            Tile_region.columns, Tile_region.rows
        FROM Tile_Region INNER JOIN Sprite ON Tile_Region.name = Sprite.name;
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
        sprite_w = sqlite3_column_int(stmt, column++);
        sprite_h = sqlite3_column_int(stmt, column++);
        columns = sqlite3_column_int(stmt, column++);
        rows = sqlite3_column_int(stmt, column++);
    });

    Tile::ID next_id = 0;
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            int x = origin_x + column * sprite_w;
            int y = origin_y + row * sprite_h;
            Tile::ID id = ++next_id;

            auto pair = std::make_pair<>(id, SpriteImpl{});
            pair.second.set_texcoords({x, y, sprite_w, sprite_h})
                .set_offset(sprite_w / 2, (sprite_h / 2) + config::cellh / 2)
                .set_position({0, 0})
                .set_size(sprite_w, sprite_h)
                .set_layer(config::tile_layer);
            sprites.emplace(pair);
        }
    }
}

Tile TileFactory::create(RenderSystem & rs, Tile::ID id) const {
    Tile tile{id};
    try {
        tile.sprite = spritef.create_from_impl(rs, &sprites.at(id));
    }
    catch (std::out_of_range) {
        std::cerr << "\nERROR: TileFactory::create_unmanaged(" << id << ")\n" << std::endl;
        throw;
    }
    return tile;
}

Tile TileFactory::create(Tile::ID id) const {
    return create(default_rs, id);
}

std::vector<Tile::ID> TileFactory::get_all() const {
    std::vector<Tile::ID> v;
    for (auto & pair : sprites) {
        v.push_back(pair.first);
    }
    return v;
}
