#include "tile.hpp"

Tile::Tile(gfx::SpriteManager & spritem) : MapObject{spritem} {
    static const int dirs[4][2] = {
        {0, -1}, // up
        {1,  0}, // right
        {0, -1}, // down
        {-1, 0}, // left
    };
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {

            Cell * cell = get_cell(x, y);

            for (int i = 0; i < DIRECTIONS; i++) {
                const auto dir = dirs[i];

                int dx = x + dir[0];
                int dy = y + dir[1];

                if (dx < 0 || dx >= cols) {
                    continue;
                }
                if (dy < 0 || dy >= rows) {
                    continue;
                }

                Cell * ncell = get_cell(dx, dy);
                cell->set_neighbor(dir[0], dir[1], ncell);
            }
        }
    }
}

void Tile::set_coordinate(const sf::Vector2f & c) {
    for (auto y = 0; y < rows; y++) {
        for (auto x = 0; x < cols; x++) {
            auto & cell = cells[y * cols + x]; // row-major order
            cell.set_coordinate({c.x + float(x), c.y + float(y)});
        }
    }
    MapObject::set_coordinate(c);
}

Cell * Tile::get_cell(const sf::Vector2f & c) {
    assert(c.x >= 0 && c.x <= cols);
    assert(c.y >= 0 && c.y <= rows);
    // NOTE: this refers to local coords from 0->cols,0->rows
    // TODO: find a way to ensure no logical bugs are caused by calling this
    // and similar methods using local coordinates
    return &cells[c.y * cols + c.x];
}

Cell * Tile::get_cell(int x, int y) {
    assert(x >= 0 && x < cols);
    assert(y >= 0 && y < rows);
    // NOTE: this refers to local coords from 0->cols,0->rows
    // TODO: find a way to ensure no logical bugs are caused by calling this
    // and similar methods using local coordinates
    return &cells[y * cols + x];
}

bool Tile::contains(const sf::Vector2f & c) const {
    auto coord = c + coordinate();
    sf::FloatRect box{coord.x, coord.y, 1.0, 1.0};
    return box.contains(c);
}

void Tile::set_neighbor(int x, int y, Tile * tile) {
    int idx = 0;
    if (y == -1) {
        neighbors[0] = tile;

        int cx = 0;
        int cy = 0;
        for (; cx < cols; cx++) {
            auto cell = get_cell(cx, cy);
            auto ncell = tile->get_cell(cx, rows - 1);
            cell->set_neighbor(0, -1, ncell);
        }
    }
    else if (x == 1) {
        neighbors[1] = tile;

        int cx = cols - 1;
        int cy = 0;
        for (; cy < rows; cy++) {
            auto cell = get_cell(cx, cy);
            auto ncell = tile->get_cell(0, cy);
            cell->set_neighbor(1, 0, ncell);
        }
    }
    else if (y == 1) {
        neighbors[2] = tile;

        int cx = 0;
        int cy = rows - 1;
        for (; cx < cols; cx++) {
            auto cell = get_cell(cx, cy);
            auto ncell = tile->get_cell(cx, 0);
            cell->set_neighbor(0, 1, ncell);
        }
    }
    else {
        neighbors[3] = tile;

        int cx = 0;
        int cy = 0;
        for (; cy < rows; cy++) {
            auto cell = get_cell(cx, cy);
            auto ncell = tile->get_cell(cols - 1, cy);
            cell->set_neighbor(-1, 0, ncell);
        }
    }
    
}
