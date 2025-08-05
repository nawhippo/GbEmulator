#include <stdio.h>
#include <cstdint>
#include <algorithm>
#include <tile.h>
class GPU {
static const uint8_t VRAM_BEGIN = 0x8000;
static const uint8_t VRAM_END = 0x9FFF;
static const uint8_t VRAM_SIZE = VRAM_END - VRAM_BEGIN + 1;
uint8_t vram[VRAM_SIZE];
Tile tile_set[384];







};
