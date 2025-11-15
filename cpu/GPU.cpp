#include <stdio.h>
#include <cstdint>
#include <algorithm>
#include "Singleton.h"
#include <SDL.h>
class GPU {
//vram should be a part of the memory bus 
Singleton& singleton = Singleton::getInstance();
uint8_t* memorybus = singleton.getMemoryBus();
uint8_t* VRAM = singleton.getVRAM();
SDL_Renderer* renderer;
SDL_Surface* screen;
SDL_Event event;
SDL_Texture* texture;
SDL_Window* window = 
uint8_t* OAM = singleton.getOAM();
uint8_t* lcdc = singleton.getLCDC();
























//presumably loading tiles 
//tile ram is accessed at addresses $9800-98FF
//oam $8000-8FFF w/ unsigned numbering 

//40 sprites (160 bytes) in oam - same format as bg tiles 
//all have 4 bytes determining object attributes
//byte 0 - y pos + 16
//byte 1 - x pos + 8
//byte 2 - Tile Index
//byte 3 - Attributes(palette, flip, priority)
//attr flags  7 priority 6 y flip 5 x flip 4 dmg palette 3 bank 2 1 0 - cgb paletted 


//Priority definitions 
//1 object is ignored when more than 10 are displayed on the scanline
//2 determiens whether it overlaps when sprite space is occupied by more than one object - how is it layered 


//draw priority 
//in non cgb mode the smaller the x coord the higher the priority when xs are identical the object located first in oam has higher prior
//in cgb mode only the location in oam determines priority 


//ppu functionality 


//vram tile data 
//3 blocks of tile data 
//block 0 = 8000 - 87FF objects are 0 - 127/ bg is 0 - 127, 
//block 1 = 8800 - 8FFF
//block 2 = 9000 - 97FF


//resolution is 160 x 144 pixels 

//vblank, versus rasterization

//def blocks to screen

//one tile is 16 bytes 
//it is 8x8 pixels
//per pixel there is 4 bits for color depth
//each row = 2 bytes one for bitplane 0 and 1 for bitplane 1 
//bitplane 1 is more sig than bit plane 0 - they both determine the color of a row

//when background - certain palette, when object - 0 is transparent
uint8_t translateColor(uint8_t bits, bool background){
    //color values
    switch(bits) {
    case 0:
        return 225;
    case 1:
        return 175;
    case 2:
        return 125;
    case 3:
        //MODE 2 means sprite 
        if (background == false){
            return -1;
        }
        return 0;
    }
}

//we draw row by row - how to designate what tiles to cache
//this should only be in address space (
uint8_t process_background_pixel_data(uint8_t curr, uint8_t tileindex, SDL_Renderer* renderer){
    //16 bit row.. 2 bit color
    //tiles are arranged in memory row by row for individual tiles, not gestalt scanlines.
    uint8_t* tilemapbool = (*(singleton.getLCDC()) >> 3) & 0x1;
    uint8_t* tiledatabool (*(singleton.getLCDC()) >> 4) & 0x1;
    uint8_t* tilemapchoose (*(singleton.getLCDC()) >> 6) & 0x1;
    //window scroll
    uint8_t WY = memorybus[FF4A];
    uint8_t WX = memorybus[FF4B];
    bool windowEnabled = (*(singleton.getLCDC() >> 5) & 1);

    uint8_t* mapbase;
    uint8_t* database;
    if (tilemapbool == 0){
        mapbase = 0x9800;
    }
    if (tilemapbool == 1){
        mapbase = 0x9C00;
    }
   if (tiledatapool == 0){
        database = 0x8800;
    }
    if (tiledatapool == 1){
        database = 0x8000;
    }
   
    if (windowEnabled & (tilemapbool == 1)){
        windowMapPtrBase = 0x9800; 
    } 
    if (windowEnabled & (tilemapbool == 0)){
        windowMapPtrBase = 0x9C00;
    }

    //Tomorrow deal with mappipg the window
    uint8_t* TileMapPtr = memorybus[mapbase];
    uint8_t* TileDataPtr = memorybus[database];
    uint8_t* scrollX = &singleton.getSCX();
    uint8_t* scrollY = &singleton.getSCY();
    uint8_t* WY = 
   for (int screenY = 0; screenY < height; screenY++) {
        for (int screenX = 0; screenX < wide; screenX++) {
            if ((screenX > 7) & (screenX < 166) & (screenY < 143) & (windowEnabled) & (ScreenX > WX) & (ScreenY < WY)){
                uint8_t* windowIndexMap = &memorybus[windowMpaPtrBase];
                uint8_t bgX = (screenX + scrollX) & 0xFF;
                uint8_t bgY = (screenY + scrollY) & 0xFF;
                uint8_t tileX = bgX / 8;
                uint8_t tileY = bgY / 8;
                uint8_t tileIndex = memorybus[WindowMapPtrBase + (tileY * 32 + tileX)];
                uint8_t pixelX = bgX % 8;
                uint8_t pixelY = bgY % 8;
            }
            uint8_t bgX = (screenX + scrollX) & 0xFF;
            uint8_t bgY = (screenY + scrollY) & 0xFF;
            uint8_t tileX = bgX / 8;
            uint8_t tileY = bgY / 8;
            //because the window isn't visible below coordinate 7
            uint8_t tileIndex = TileMapPtr[tileY * 32 + tileX];
            uint8_t pixelX = bgX % 8;
            uint8_t pixelY = bgY % 8;
            
            // NOW use the 16-byte offset to get tile data  
            uint16_t tileDataAddr = database + (tileIndex * 16) + (pixelY * 2);
            uint8_t lo = memorybus[tileDataAddr];
            uint8_t hi = memorybus[tileDataAddr + 1];
            
            // Extract pixel
            int bit = 7 - pixelX;
            int color = ((hi >> bit) & 1) << 1 | ((lo >> bit) & 1);
            SDL_RenderDrawPoint(renderer, screenX, screenY);
        }
    }
}

uint8_t process_object_pixel_data(int8_t curr, uint8_t tileindex, SDL_Renderer* renderer){
    //this should run every draw cycle
    //this is the current tile in vram
    //little endian
    int wide = 160;
    int height = 144; 

    //16 bit row.. 2 bit color
    uint8_t* scrollx = singleton.getSCX();
    scrolly = singleton.getSCY();
    uint8_t* tilemapbool = (singleton.getLCDC() >> 3) & 1;
    uint8_t* tiledatabool (singleton.getLCDC() >> 4) & 1;
    uint8_t mapbase;
    uint8_t database;
    if (tilemapbool == 0){
        mapbase = 0x9800;
    }
    if (tilemapbool == 1){
        mapbase = 0x9C00;
    }

    //tile data for objects is always 8000
    database = 0x8000
    TileMapPtr = &memorybus[mapbase];
    OAMblock = singleton.getOAM();
    for (int y = 0; y < height; y++){
        //we dr
            for (int x = 0; x < wide; x++){
                //no palettes being used in non gbc mode
                //this is 8 pixels, not 16 - they are combined. 
                bgx = (x + Scrollx) & 0xFF;
                bgy = (y + Scrolly) & 0xFF;
                tilex = bgx / 8;
                tiley = bgy / 8;
                //16x16 bits for a two bit color depth - the memory unit is uint8_t so /8 hence 32 tile y.
                //32 x 32 tile index grid (each tile index is 1 byte and there are 32 tiles in a row so)
                uint8_t tileIndex = TileMapPtr[tiley * 32 + tilex]; 
                //16 bytes for tile
                uint8_t *tileDataPtr = memorybus[0x8000 + tileIndex * 16];

                get_color 



                pixelx = bgx % 8;
                pixely = bgy % 8;
                uint8_t lo = oam[curr];
                uint8_t hi = oam[curr+1];
                uint8_t* OAMBlock = singleton.getOAM();
                uint16_t OAMData = *(OAMBlock + index) << 8 & *(OAMBlock + 1 + index); 
                uint8_t y_pos = OAMData >> 8 & 0xFF;
                uint8_t x_pos = OAMData & 0xFF;
                uint8_t attributes = OAMBlock[index + 2];
                uint8_t priority = attributes >> 7 & 0b1;
                uint8_t y_flip = attributes >> 6 & 0b1;
                uint8_t x_flip = attributes >> 5 & 0b1;
                uint8_t bank = attributes >> 3 & 0b1;
                uint8_t bgX = (x + scrollX) & 0xFF;
                uint8_t bgY = (y + scrollY) & 0xFF;
                int bit = 7 - x;

                //colors are 2bit values - row data is a series of
                //this boils it down to just 2 bit
                int color = ((hi >> bit) & 1) << 1 | (( >> bit) & 1);
  


                //handle priority
                if (dmg_palette == 0) {
                    int color = translateColor(color, false);
                } else {
                    int color = translateColor(color, true);
                }
                


                int color = translateColor(color, false);
                if (color == -1){
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, color, 255);
                }
                //16 bits per palette. 
                x_coord = x_flip ? wide - x : x;
                y_coord = y_flip ? height - y : y;
                
                SDL_RenderDrawPoint(renderer, x, y);
                if (x % 16 == 0){
                    curr+=2; 
                };
    };
};
}

uint8_t process_window_data(uint8_t curr){
    //if bit 1 is set in non gbc mode (default) and bit 5 generally.
    bool windowDisplayed = ((*(lcdc) >> 5) & 0x1) & (*(lcdc) & 0x1);
    uint8_t *windowYScroll = memorybus[FF4A];
    uint8_t *windowXScroll = memorybus[FF4B];
    
    if (windowDisplayed == True){
        for (int y = 0; y < 144; y++){
            for (int x = 0; x < 160; x++){
                

}
        }}}

//the bg tilemap is 256 x 256 pixels - 32 x 32 tiles
//scroll x and y are background tile coords

//we read scx and scy from io registers
//we also check lcdc register to see which tile map to use
//then the resultant address is where we begin processing pixels. 
uint8_t get_tilerow_index(uint16_t address, uint8_t scrollX){
    //tile indexes map to an addy in pure vram data. 
    uint16_t addy = (*singleton.getSCX() + (*singleton.getSCY())) % 8;
    return addy;
}



int init_SDL(int argc, char* argv[]){
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) {
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    //4 bit pixel depth settings 
    screen = SDL_SetVideoMode(640, 480, 4, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL){
        printf(stderror, "Couldn't initialize 640x480x4 video made: %s\n", SDL_GetError());
        exit(-1);
    }   
    window = SDL_CreateWindow("Session",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SL);
    return 0; 
    }
};




//dots are hoz pizel lines
//FF46 writes from ROM/RAM to OAM
//DMA takes 160-M cyclees 640 dots
//rom and WRAM are on seperate cartridges 
//blocks OAM reads
//busy-wait in hram because call makes return address on stack
//do not activate during rendering or incorrecot tiel numbers will be fetched, becuase the ppu will read the dma writes as opposed to oam data itself.
//do not read during oam scan as it will hide it and count it as off screen

int dma_transfer(){
    uint8_t starting_address_hi = memorybus[0xFF46];
    uint16_t transfer_start_address = starting_address_hi << 8;
    uint8_t* bus = singleton.getMemoryBus();
    uint8_t* curr_oam = singleton.getOAM();
    while (transfer_start_address < transfer_start_address + 0x009F){
        uint8_t data = bus[transfer_start_address];
        *curr_oam = data;
        curr_oam++;
        transfer_start_address++;
    }
}
