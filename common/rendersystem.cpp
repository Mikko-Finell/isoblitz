#include "rendersystem.hpp"
#include "sprite.hpp"

namespace {
bool operator>(const SpriteData & a, const SpriteData & b);
bool operator<(const SpriteData & a, const SpriteData & b);
inline void vert_set_pos(sf::Vertex * vs, const sf::IntRect & rect);
inline void vert_set_crd(sf::Vertex * vs, const sf::IntRect & rect);
}

void WorldRender::add(SpriteData & data) {
    spritedata.insert(&data);
}

void WorldRender::remove(SpriteData & data) {
    spritedata.erase(&data);
}

void WorldRender::draw(sf::RenderWindow & window) {
    auto view = window.getView();
    auto center = sf::Vector2i(view.getCenter());
    auto size = sf::Vector2i(view.getSize());
    auto pos = sf::Vector2i(center - size / 2);
    sf::IntRect screen{pos, size};

    static std::vector<SpriteData *> visible_sprites;
    visible_sprites.clear();
    for (auto & sprite : spritedata) {
        if (sprite->screencoords.intersects(screen)) {
            visible_sprites.push_back(sprite);
        }
    }
    auto cmp = [this](const SpriteData * lhs, const SpriteData * rhs){
        return *lhs < *rhs;
    };
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

// UIRender /////////////////////////////////////////////////////////////////////

void UIRender::add(SpriteData & data) {
    if (keys.insert(&data).second) {
        spritedata.push_back(&data);
        auto cmp = [](const SpriteData * lhs, const SpriteData * rhs){
            return lhs->layer < rhs->layer;
        };
        std::sort(spritedata.begin(), spritedata.end(), cmp);
    }
}

void UIRender::remove(SpriteData & data) {
    // unordered_set::erase(key) returns number of elements erased
    if (keys.erase(&data)) {
        spritedata.erase(std::remove(spritedata.begin(), spritedata.end(),
                         &data), spritedata.end());
    }
}

void UIRender::draw(sf::RenderWindow & window) {
    static std::vector<sf::Vertex> vs;
    vs.resize(4 * spritedata.size());

    std::size_t idx = 0;
    const auto vertex_count = spritedata.size() * 4;
    for (auto itr = spritedata.begin(); idx < vertex_count; idx += 4) {
        SpriteData * sd = *itr;
        vert_set_pos(&vs[idx], sd->screencoords);
        vert_set_crd(&vs[idx], sd->spritecoords);
        ++itr;
    }

    auto view = window.getView();
    window.setView(window.getDefaultView());
    window.draw(&vs[0], idx, sf::Quads, &texture);
    window.setView(view);
}

// Utility functions ////////////////////////////////////////////////////////////

namespace {
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
}
