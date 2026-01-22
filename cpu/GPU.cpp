#include <stdio.h>
#include <cstdint>
#include <algorithm>
#include "Singleton.h"
#include <SDL2.h>
class GPU {
Singleton& singleton = Singleton::getInstance();
uint8_t* memorybus = singleton.getMemoryBus();
uint8_t* VRAM = singleton.getVRAM();
Accumulator& ppuAccumulator = Accumulators['h'];
SDL_Renderer* renderer;
SDL_Surface* screen;
SDL_Event event;
SDL_Texture* texture;
SDL_Window* window = nullptr;
uint8_t* OAM = singleton.getOAM();
uint8_t* lcdc = singleton.getLCDC();
uint8_t* STAT = memory[0xFF41];
//only during this can cpu access vram
uint8_t translateColor(uint8_t bits, bool background){
    switch(bits) {
    case 0:
        return 225;
    case 1:
        return 175;
    case 2:
        return 125;
    case 3:
        if (background == false){
            return 255;
        }
        return 0;
    default:
        return 0;
    }
}

uint8_t process_background_pixel_data(uint8_t curr, uint8_t tileindex, SDL_Renderer* renderer){
    uint8_t tilemapbool = (*lcdc >> 3) & 0x1;
    uint8_t tiledatabool = (*lcdc >> 4) & 0x1;
    uint8_t tilemapchoose = (*lcdc >> 6) & 0x1;
    uint8_t WY = memorybus[0xFF4A];
    uint8_t WX = memorybus[0xFF4B];
    bool nonGBCModeWindowToggle = (*lcdc & 1);
    bool windowEnabled = (*lcdc >> 5) & 1;

    uint16_t mapbase;
    uint16_t database;
    if (tilemapbool == 0){
        mapbase = 0x9800;
    }
    if (tilemapbool == 1){
        mapbase = 0x9C00;
    }
    if (tiledatabool == 0){
        database = 0x8800;
    }
    if (tiledatabool == 1){
        database = 0x8000;
    }
    
    uint16_t windowMapPtrBase = 0x9800;
    if (windowEnabled & (tilemapbool == 1) & nonGBCModeWindowToggle){
        windowMapPtrBase = 0x9800; 
    } 
    if (windowEnabled & (tilemapbool == 0) & nonGBCModeWindowToggle){
        windowMapPtrBase = 0x9C00;
    }

    uint8_t* TileMapPtr = &memorybus[mapbase];
    uint8_t* TileDataPtr = &memorybus[database];
    uint8_t* scrollX = singleton.getSCX();
    uint8_t* scrollY = singleton.getSCY();
    int wide = 160;
    int height = 144;
    
    for (int screenY = 0; screenY < height; screenY++) {
        for (int screenX = 0; screenX < wide; screenX++) {
            if ((screenX > 7) & (screenX < 166) & (screenY < 143) & (windowEnabled) & (screenX > WX) & (screenY < WY)){
                uint8_t* windowIndexMap = &memorybus[windowMapPtrBase];
                uint8_t bgX = (screenX + *scrollX) & 0xFF;
                uint8_t bgY = (screenY + *scrollY) & 0xFF;
                uint8_t tileX = bgX / 8;
                uint8_t tileY = bgY / 8;
                uint8_t pixelX = bgX % 8;
                uint8_t pixelY = bgY % 8;
                uint8_t tileIndex = windowIndexMap[tileY * 32 + tileX];
                uint16_t tileDataAddr = database + (tileIndex * 16) + (pixelY * 2);
                uint8_t lo = memorybus[tileDataAddr];
                uint8_t hi = memorybus[tileDataAddr + 1];
                int bit = 7 - pixelX;
                uint8_t color = (((hi >> bit) & 1) << 1 | ((lo >> bit) & 1)) & 0xFF;
                color = translateColor(color, true);
                SDL_SetRenderDrawColor(renderer, color, color, color, 255);
                SDL_RenderDrawPoint(renderer, screenX, screenY);
            } else {
                uint8_t bgX = (screenX + *scrollX) & 0xFF;
                uint8_t bgY = (screenY + *scrollY) & 0xFF;
                uint8_t tileX = bgX / 8;
                uint8_t tileY = bgY / 8;
                uint8_t tileIndex = TileMapPtr[tileY * 32 + tileX];
                uint8_t pixelX = bgX % 8;
                uint8_t pixelY = bgY % 8;
                
                uint16_t tileDataAddr = database + (tileIndex * 16) + (pixelY * 2);
                uint8_t lo = memorybus[tileDataAddr];
                uint8_t hi = memorybus[tileDataAddr + 1];
                
                int bit = 7 - pixelX;
                int color = ((hi >> bit) & 1) << 1 | ((lo >> bit) & 1);
                color = translateColor(color, true);
                SDL_SetRenderDrawColor(renderer, color, color, color, 255);
                SDL_RenderDrawPoint(renderer, screenX, screenY);
            }
        }
    }
    STAT* = 3;
    return 0;
}

uint8_t process_object_pixel_data(int8_t curr, uint8_t tileindex, SDL_Renderer* renderer){
    int wide = 160;
    int height = 153; 

    uint8_t* scrollX = singleton.getSCX();
    uint8_t* scrollY = singleton.getSCY();
    uint8_t tilemapbool = (*lcdc >> 3) & 1;
    uint8_t tiledatabool = (*lcdc >> 4) & 1;
    uint16_t mapbase;
    uint16_t database;
    if (tilemapbool == 0){
        mapbase = 0x9800;
    }
    if (tilemapbool == 1){
        mapbase = 0x9C00;
    }

    database = 0x8000;
    uint8_t* TileMapPtr = &memorybus[mapbase];
    uint8_t* OAMblock = singleton.getOAM();
    
    for (int screenY = 0; screenY < height; screenY++){
        for (int screenX = 0; screenX < wide; screenX++){
    
            uint8_t bgX = (screenX + *scrollX) & 0xFF;
            uint8_t bgY = (screenY + *scrollY) & 0xFF;
            uint8_t tileX = bgX / 8;
            uint8_t tileY = bgY / 8;
            uint8_t tileIndex = TileMapPtr[tileY * 32 + tileX]; 
            uint8_t *tileDataPtr = &memorybus[0x8000 + tileIndex * 16];

            uint8_t pixelX = bgX % 8;
            uint8_t pixelY = bgY % 8;
            uint8_t lo = OAMblock[curr];
            uint8_t hi = OAMblock[curr+1];
            uint8_t* OAMBlock = singleton.getOAM();
            uint16_t OAMData = (*(OAMBlock + curr) << 8) | *(OAMBlock + 1 + curr); 
            uint8_t y_pos = (OAMData >> 8) & 0xFF;
            uint8_t x_pos = OAMData & 0xFF;
            uint8_t attributes = OAMBlock[curr + 2];
            uint8_t priority = (attributes >> 7) & 0b1;
            uint8_t y_flip = (attributes >> 6) & 0b1;
            uint8_t x_flip = (attributes >> 5) & 0b1;
            uint8_t bank = (attributes >> 3) & 0b1;
            int bit = 7 - pixelX;

            int color = ((hi >> bit) & 1) << 1 | ((lo >> bit) & 1);

            color = translateColor(color, false);
            if (color == -1){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, color, 255);
            }
            int x_coord = x_flip ? wide - screenX : screenX;
            int y_coord = y_flip ? height - screenY : screenY;
            
            SDL_RenderDrawPoint(renderer, screenX, screenY);
        }
    }
    STAT* = 1;
    return 0;
}

int init_SDL(int argc, char* argv[]){
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) {
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    
    window = SDL_CreateWindow("Game Boy Emulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        exit(-1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        exit(-1);
    }
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 160, 144);
    return 0; 
    }
};

int dma_transfer(){
    uint8_t starting_address_hi = memorybus[0xFF46];
    uint8_t starting_address_lo = memorybus[0xFF46] + 1;
    uint16_t transfer_start_address = (starting_address_hi << 8) & starting_address_lo;
    uint8_t* bus = singleton.getMemoryBus();
    uint8_t* curr_oam = singleton.getOAM();
    while (transfer_start_address < transfer_start_address + 0x009F){
        uint8_t data = bus[transfer_start_address];
        *curr_oam = data;
        curr_oam++;
        transfer_start_address++;
    }
}
