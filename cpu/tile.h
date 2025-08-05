#ifndef TILE_H
#define TILE_H
#include <cstdint>
class Tile {
    public:
    enum TilePixelValue : uint8_t;
    Tile* init_tile();
    TilePixelValue arr[8][8];
};
#endif