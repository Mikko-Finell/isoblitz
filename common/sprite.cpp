#include "sprite.hpp"
#include "util.hpp"
#include <sqlite3.h>
#include <cassert>
#include <iostream>

bool operator>(const SpriteData & a, const SpriteData & b);
bool operator<(const SpriteData & a, const SpriteData & b);

inline void vert_set_pos(sf::Vertex * vs, const sf::IntRect & rect) {
    vs[0].position.x = rect.left;
    vs[0].position.y = rect.top;
    vs[1].position.x = rect.left + rect.width;
    vs[1].position.y = rect.top;
    vs[2].position.x = rect.left + rect.width;
    vs[2].position.y = rect.top + rect.height;
    vs[3].position.x = rect.left;
    vs[3].position.y = rect.top + rect.height;
}

inline void vert_set_crd(sf::Vertex * vs, const sf::IntRect & rect) {
    vs[0].texCoords.x = rect.left;
    vs[0].texCoords.y = rect.top;
    vs[1].texCoords.x = rect.left + rect.width;
    vs[1].texCoords.y = rect.top;
    vs[2].texCoords.x = rect.left + rect.width;
    vs[2].texCoords.y = rect.top + rect.height;
    vs[3].texCoords.x = rect.left;
    vs[3].texCoords.y = rect.top + rect.height;
}

SpriteManager::SpriteManager() {
    const auto sqlquery = R"(
        SELECT sprite.name, entity.name,
            origin_x+x, origin_y+y, w, h, offset_x, offset_y
        FROM entity INNER JOIN sprite
        ON entity.name = sprite.entity
        WHERE sprite.frames IS NULL
    )";
    sqlite3 * db;
    sqlite3_stmt * stmt;

    assert(sqlite3_open("../data/testdb.sqlite3", &db) == SQLITE_OK);
    if (sqlite3_prepare(db, sqlquery, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "SQL Error from SpriteManager, sqlite3_prepare: "
                  << sqlite3_errmsg(db) << std::endl;
        return;
    }

    int result_code = sqlite3_step(stmt);
    while (result_code == SQLITE_ROW) {
        int column = 0;

        std::string sprite_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };
        std::string entity_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };

        const int x = sqlite3_column_int(stmt, column++);
        const int y = sqlite3_column_int(stmt, column++);
        const int w = sqlite3_column_int(stmt, column++);
        const int h = sqlite3_column_int(stmt, column++);
        const int ox = sqlite3_column_int(stmt, column++);
        const int oy = sqlite3_column_int(stmt, column++);

        auto & spritemap = entitymap[entity_name];
        auto & spritedata = spritemap[sprite_name];

        spritedata.offset = sf::Vector2i{ox, oy};
        spritedata.spritecoords.left = x;
        spritedata.spritecoords.top = y;
        spritedata.spritecoords.width = w;
        spritedata.spritecoords.height = h;
        spritedata.screencoords.width = w;
        spritedata.screencoords.height = h;

        result_code = sqlite3_step(stmt);
    }
    if(result_code != SQLITE_DONE) {
        std::cerr << "SQL Error: SpriteManager, sqlite3_step: "
            << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void SpriteManager::add(SpriteData & data) {
    spritedata.insert(&data);
}

SpriteData
SpriteManager::get(const std::string & entity, const std::string & sprite) {
    SpriteData data;
    try {
        data = entitymap.at(entity).at(sprite);
    }
    catch (std::out_of_range) {
        std::cerr << "\nERROR: SpriteManager::get(" << entity << ", " << sprite 
            << ")\n" << std::endl;
        throw;
    }
    return data;
}

void SpriteManager::remove(SpriteData & data) {
    spritedata.erase(&data);
}

void SpriteManager::draw(sf::RenderWindow & window) {
    auto view = window.getView();
    auto center = sf::Vector2i(view.getCenter());
    auto size = sf::Vector2i(view.getSize());
    auto pos = sf::Vector2i(center - size / 2);
    sf::IntRect screen{pos, size};

    auto cmp = [this](const SpriteData * lhs, const SpriteData * rhs){
        return *lhs < *rhs;
    };
    static std::vector<SpriteData *> visible_sprites;
    visible_sprites.clear();
    for (auto & sprite : spritedata) {
        if (sprite->screencoords.intersects(screen)) {
            visible_sprites.push_back(sprite);
        }
    }
    std::sort(visible_sprites.begin(), visible_sprites.end(), cmp);

    static std::vector<sf::Vertex> vs;
    if (vs.size() < 4 * visible_sprites.size()) {
        vs.resize(4 * visible_sprites.size());
    }

    std::size_t idx = 0;
    const auto vertex_count = visible_sprites.size() * 4;
    for (auto itr = visible_sprites.begin(); idx < vertex_count; idx += 4) {
        SpriteData * sd = *itr;
        vert_set_pos(&vs[idx], sd->screencoords);
        vert_set_crd(&vs[idx], sd->spritecoords);
        ++itr;
    }
    window.draw(&vs[0], idx, sf::Quads, &texture);
}

// Sprite ///////////////////////////////////////////////////////////////////////

Sprite::Sprite() {
}

Sprite::Sprite(SpriteManager & sm) {
    init(sm);
}

Sprite::Sprite(const Sprite & other) {
    this->data = other.data;
    init(other.spritem);
}

Sprite & Sprite::operator=(const Sprite & other) {
    this->data = other.data;
    init(other.spritem);
    return *this;
}

Sprite & Sprite::operator=(const SpriteData & data) {
    this->data = data;
    return *this;
}

Sprite::~Sprite() {
    if (spritem) {
        spritem->remove(data);
    }
}

Sprite & Sprite::init(SpriteManager * sm) {
    if (sm) {
        init(*sm);
    }
    return *this;
}

Sprite & Sprite::init(SpriteManager & sm) {
    spritem = &sm;
    show();
    return *this;
}

Sprite & Sprite::show() {
    assert(spritem);
    spritem->add(data);
    visible = true;
    return *this;
}

Sprite & Sprite::hide() {
    assert(spritem);
    spritem->remove(data);
    visible = false;
    return *this;
}

Sprite & Sprite::set_position(int x, int y) {
    data.screencoords.left = x - data.offset.x;
    data.screencoords.top = y - data.offset.y;
    return *this;
}

Sprite & Sprite::set_size(int w, int h) {
    data.screencoords.width = w;
    data.screencoords.height = h;
    return *this;
}

Sprite & Sprite::set_screencoords(const sf::IntRect & coords) {
    data.screencoords = coords;
    return *this;
}

Sprite & Sprite::set_spritecoords(const sf::IntRect & coords) {
    data.spritecoords = coords;
    return *this;
}

Sprite & Sprite::set_spritecoord(const sf::Vector2i & coords) {
    return set_spritecoord(coords.x, coords.y);
}

Sprite & Sprite::set_spritecoord(int x, int y) {
    data.spritecoords.left = x;
    data.spritecoords.top = y;
    return *this;
}

Sprite & Sprite::set_data(const SpriteData & d) {
    data = d;
    return *this;
}

Sprite & Sprite::set_offset(int x, int y) {
    data.offset = sf::Vector2i{x, y};
    return *this;
}

void Sprite::serialize(std::ostream & out) const {
    util::write(data.screencoords.left, out);
    util::write(data.screencoords.top, out);
    util::write(data.screencoords.width, out);
    util::write(data.screencoords.height, out);

    util::write(data.spritecoords.left, out);
    util::write(data.spritecoords.top, out);

    util::write(data.layer, out);

    util::write(data.offset.x, out);
    util::write(data.offset.y, out);

    util::write(visible, out);
}

void Sprite::deserialize(std::istream & in) {
    util::read(data.screencoords.left, in);
    util::read(data.screencoords.top, in);
    util::read(data.screencoords.width, in);
    util::read(data.screencoords.height, in);

    util::read(data.spritecoords.left, in);
    util::read(data.spritecoords.top, in);

    util::read(data.layer, in);

    util::read(data.offset.x, in);
    util::read(data.offset.y, in);

    util::read(visible, in);
}

Sprite & Sprite::set_layer(int z) {
    data.layer = z;
    return *this;
}

bool Sprite::operator==(const Sprite & other) const {
    return data.spritecoords == other.data.spritecoords &&
        data.screencoords == other.data.screencoords &&
        data.layer == other.data.layer;
}

bool operator>(const SpriteData & a, const SpriteData & b) {
    if (a.layer == b.layer) {
        if (a.screencoords.top == b.screencoords.top) {
            return a.screencoords.left >= b.screencoords.left;
        }
        else {
            return a.screencoords.top > b.screencoords.top;
        }
    }
    else {
        return a.layer > b.layer;
    }
}

bool operator<(const SpriteData & a, const SpriteData & b) {
    return !(a > b);
}
