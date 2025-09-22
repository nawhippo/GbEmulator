#include <stdio.h>
#include <cstdint>
#include <algorithm>
#include <tile.h>
#include <SDL.h>
class GPU {
Tile tile_set[384];
//vram should be a part of the memory bus 

Singleton& singleton = singleton.getInstance();
uint8_t* VRAM = singleton.getVRAM();



//presumably loading tiles 
//tile ram is accessed at addresses $9800-98FF
//oam $8000-8FFF w/ unsigned numbering 

//40 sprites in oam - same format as bg tiles 
//all have 4 bytes determining object attributes
//byte 0 - y pos
//byte 1 - x pos
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


uint8_t translateColor(uint8_t 2bits){
    switch(2bit) {
    case 0:
        return 225;
    case 1:
        return 175;
    case 2:
        return 125;
    case 3:
        return 0;
    }

//we draw row by row 
uint8_t drawTile(uint8_t curr){
    //this is the current tile in vram
    //little endian
    bool background = false;
    if (curr > 0x9799 and curr < 0x9BFF - 0x0001){
        background = true;
    }
    int wide = 160;
    int height =  144; 
    //16 bit row.. 2 bit color
    for (int y = 0; y < 144; y++){
            for (int x = 0; x < 161; x++){
            uint8_t lo = VRAM[curr];
            uint8_t hi = VRAM[curr+1];
            int bit = 7 - i;
            int color = ((hi >> bit) & 1) << 1 | ((low >> bit) & 1);
            int color = translateColor(color);
            if (x % 8 == 0){

                curr+=2;
                //after translating the 2 byte into a row of color, we render it, and then empty the array
            }
    };
}
}



int init_SDL(int argc, char* argv[]){
    if((SDL_Init(SDL_INIT_VIDEO|SLD_INIT_AUDIO)==-1)) {
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    //4 bit pixel depth settings 
    screen = SDL_SetVideoMode(640, 480, 4, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL){
        fprintf(stderror, "Couldn't initialize 640x480x4 video made: %s\n", SDL_GetError());
        exit(-1);
    }
    SDL_Window *win = SDL_CreateWindow("Session",
        SDL_WINDOWPOS_CENTERED, SDLWINDOWPOS_CENTERED, 640, 480 0);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    exit(0); 
    }


















};
}