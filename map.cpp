#include <algorithm>
#include "map.hpp"

void Map::create_at(const sf::Vector2f & pos) {
    tiles.push_back(Tile::centered_at(pos));
}

void Map::remove_at(const sf::Vector2f & pos) {
}

void Map::draw(std::vector<sf::Vertex> & vertices) {
    std::sort(tiles.begin(), tiles.end());
    for (auto & tile : tiles) {
	tile.draw(vertices);
    }
}
