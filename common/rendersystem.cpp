#include "rendersystem.hpp"
#include "sprite.hpp"

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


void RenderSystem::add(SpriteData & data) {
    spritedata.insert(&data);
}

void RenderSystem::remove(SpriteData & data) {
    spritedata.erase(&data);
}

void RenderSystem::draw(sf::RenderWindow & window) {
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
