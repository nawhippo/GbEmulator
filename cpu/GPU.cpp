#include <stdio.h>
#include <cstdint>
#include <algorithm>
#include "Singleton.h"
#include <SDL.h>
class GPU {
//vram should be a part of the memory bus 
Singleton& singleton = singleton.getInstance();
uint8_t* VRAM = singleton.getVRAM();
SDL_Renderer* renderer;
SDL_Surface* screen;
SDL_Event event;
SDL_Texture* texture;
SDL_WWindow* window;
uint8_t* OAM = singleton.getOAM();
uint8_t LCDC = *singleton.lcdc;
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
uint8_t translateColor(uint8_t 2bits, bool background){
    //color values
    switch(2bit) {
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

//we draw row by row - how to designate what tiles to cache
//this should only be in address space (
uint8_t Process_Pixel_Data(uint8_t curr, uint8_t tileindex, SDL_Renderer* renderer){
    //this should run every draw cycle
    //this is the current tile in vram
    //little endian
    int wide = 160;
    int height = 144; 
    //16 bit row.. 2 bit color
    for (int y = 0; y < 144; y++){
        //we dr
            for (int x = 0; x < 160; x++){
                //this is 8 pixels, not 16 - they are combined. 
                uint8_t lo = VRAM[curr];
                uint8_t hi = VRAM[curr+1];
                int bit = 7 - x;
                //colors are 2bit values - row data is a series of 
                //this boils it down to just 2 bits 
                int color = ((hi >> bit) & 1) << 1 | ((low >> bit) & 1);
                int color = translateColor(color, background);
                if (color == -1){
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, color, 255);
                }
                //WHEN WE DRAW SPRITES WE NEED TO CHECK PRIOEIRT 
                //color indices are palettes - 16 bits per palette. 
                SDL_RenderDrawPoint(renderer, x, y);
                if (x % 16 == 0){
                    curr+=2; 
                };
    };
};
}


//the bg tilemap is 256 x 256 pixels - 32 x 32 tiles
//scroll x and y are background tile coords

//we read scx and scy from io registers
//we also check lcdc register to see which tile map to use
//then the resultant address is where we begin processing pixels. 
uint8_t get_tilerow_index(uint16_t address, uint8_t scrollX, singleton.){
    //tile indexes map to an addy in pure vram data. 
    uint16_t addy = (scrollX + scrollY) % 8;
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
    this.window = SDL_CreateWindow("Session",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    this.renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    this.texture = SDL_CreateTexture(renderer, SL);
    return 0; 
    }
};




int read_oam(int index){
    //each object is 4 bytes 
    //byte 1 x pos
    //byte 2 tile ind
    //byte 3 attributes
    //7 priority 6 y flip 5 x flip 4 dmg palette 3 bank (only in cgb NA) 2 1 0 - cgb paletted
    uint8_t* OAM = singleton.getOAM();
    uint8_t y_pos = OAM[index];
    uint8_t x_pos = OAM[index + 1];
    uint8_t attributes = OAM[index + 2];

    uint8_t priority = attributes >> 7 & 0b1;
    uint8_t y_flip = attributes >> 6 & 0b1;
    uint8_t x_flip = attributes >> 5 & 0b1;
    uint8_t dmg_palette = attributes >> 4 & 0b1;
    uint8_t bank = attributes >> 3 & 0b1;
    uint8_t cgb_palette = attributes & 0b111;
    return 0;
}




















}
