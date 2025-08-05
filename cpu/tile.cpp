#include <stdio.h>
#include <cstdint>
#include <algorithm>

class Tile { 

enum TilePixelValue : uint8_t {
    Zero,
    One,
    Two,
    Three,
};

TilePixelValue arr[8][8];

//tile factory function
Tile* init_tile() {
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            arr[i][j] = Zero;
    }
}
    return this;
}



};