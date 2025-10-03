#include <singleton.h>
#include <stdio.h>
#include <iostream>
class MemoryBus
{
    // when is this initialized?
    Singleton &singleton = singleton.getInstance();
    uint8_t Memory[0xFFFF];
    // addys for grahpics
    //$8000 - 97FF is VRAM each tile takes up 16 bytes

    // vram 0x8000 - 0x9FFF
    //$8000 addressing 0 - 127 in block 0 and 128 - 255 in block 1
    //$8800 - $9000 is base pointer, signed -128 to -1, 0 - 127

    // tile maps, are references to tiles, not directly written

    // window is a secondary background layer, only the top left pixel is controllable
    // it is often used for fixed ui elements like status bars

    // tiles are 8 by 8 blocks
    // object - an object attribute memory, composed of one or two tiles and can move independently

    // first byte is least sig, second is most for color coding
    // 2 bit colors means 4 possible colors
    // 00 - white, 01 - light gray, 10 - dark gray, 11 - black

    // we have to offset because memory addresses are absolute, regardless of what portion of memory they are in
    // even tho there are different banks of memory, they all have absolute addresses
    // we have to start at zero in our array so we subtract the base to remain within the arrays space.
    uint8_t &MemoryBus::readByte(uint16_t address)
    {
        if (address <= 0x3FFF)
        { // 0000–3FFF: ROM Bank 0
            return singleton.getROM()[address];
        }
        else if (address >= 0x4000 && address <= 0x7FFF)
        { // 4000–7FFF: ROM Bank N
            return singleton.getROM()[address];
        }
        else if (address >= 0x8000 && address <= 0x9FFF)
        { // 8000–9FFF: VRAM
            return singleton.getVRAM()[address - 0x8000];
        }
        else if (address >= 0xA000 && address <= 0xBFFF)
        { // A000–BFFF: External RAM
            return singleton.getRAM()[address - 0xA000];
        }
        else if (address >= 0xC000 && address <= 0xCFFF)
        { // C000–CFFF: WRAM Bank 0
            return singleton.getWRAMBank0()[address - 0xC000];
        }
        else if (address >= 0xD000 && address <= 0xDFFF)
        { // D000–DFFF: WRAM Bank 1
            return singleton.getWRAMBank1()[address - 0xD000];
        }
        else if (address >= 0xE000 && address <= 0xFDFF)
        { // E000–FDFF: Echo RAM
            return singleton.getEchoRAM()[address - 0xE000];
        }
        else if (address >= 0xFE00 && address <= 0xFE9F)
        { // FE00–FE9F: OAM
            return singleton.getOAM()[address - 0xFE00];
        }
        else if (address >= 0xFEA0 && address <= 0xFEFF)
        { // FEA0–FEFF: Unusable
            throw std::out_of_range("Unusable memory region");
        }
        else if (address >= 0xFF00 && address <= 0xFF7F)
        { // FF00–FF7F: IO Registers
            return singleton.getIORegisters()[address - 0xFF00];
        }
        else if (address >= 0xFF80 && address <= 0xFFFE)
        { // FF80–FFFE: HRAM
            return singleton.getHRAM()[address - 0xFF80];
        }
        else if (address == 0xFFFF)
        { // FFFF: Interrupt Enable
            return singleton.getInterruptEnable();
        }
        else
        {
            throw std::out_of_range("Invalid memory address");
        }
    }

    int writeByte(uint16_t address, uint8_t value)
{
    try
    {
        if (address <= 0x3FFF)
        { // 0000–3FFF: ROM Bank 0 (usually read-only, but we'll write for emulation)
            singleton.getROMBank0()[address] = value;
            return 1;
        }
        else if (address >= 0x4000 && address <= 0x7FFF)
        { // 4000–7FFF: ROM Bank N - RO, MBC COMMANDS CAN TRIGGER BANK SWITCHING
            singleton.getROMBankN()[address - 0x4000] = value;
            return 1;
        }
        else if (address >= 0x8000 && address <= 0x9FFF)
        { // 8000–9FFF: VRAM CAN WRITE WHEN LCD ALLOWS - OTHER WISE COMMANDS ARE IGNORED
            singleton.getVRAM()[address - 0x8000] = value;
            return 1;
        }
        else if (address >= 0xA000 && address <= 0xBFFF)
        { // A000–BFFF: External RAM READ/WRITE if ram enabled; some mbc control may affect access
            singleton.getRAM()[address - 0xA000] = value;
            return 1;
        }
        else if (address >= 0xC000 && address <= 0xCFFF)
        { // C000–CFFF: WRAM Bank 0 - normal/write RAM
            singleton.getWRAMBank0()[address - 0xC000] = value;
            return 1;
        }
        else if (address >= 0xD000 && address <= 0xDFFF)
        { // D000–DFFF: WRAM Bank 1 normal read/write bank switchable
            singleton.getWRAMBank1()[address - 0xD000] = value;
            return 1;
        }
        else if (address >= 0xE000 && address <= 0xFDFF)
        { // E000–FDFF: Echo RAM mirrors $C000-DDFF
            singleton.getEchoRAM()[address - 0xE000] = value;
            return 1;
        }
        else if (address >= 0xFE00 && address <= 0xFE9F)
        { // FE00–FE9F: OAM writes only allowed in certain lcd modes; outside periods ignored or glitchy. 
            singleton.getOAM()[address - 0xFE00] = value;
            return 1;
        }
        else if (address >= 0xFEA0 && address <= 0xFEFF)
        { // FEA0–FEFF: Unusable
            return -1;
        }
        else if (address >= 0xFF00 && address <= 0xFF7F)
        { // FF00–FF7F: IO Registers
            singleton.getIORegisters()[address - 0xFF00] = value;
            return 1;
        }
        else if (address >= 0xFF80 && address <= 0xFFFE)
        { // FF80–FFFE: HRAM
            singleton.getHRAM()[address - 0xFF80] = value;
            return 1;
        }
        else if (address == 0xFFFF)
        { // FFFF: Interrupt Enable
            singleton.getInterruptEnable() = value;
            return 1;
        }
        else
        {
            return -1; // invalid memory
        }
    }
    catch (...)
    {
        return -1; // any exception counts as failure
    }

    // tile set should be cached.

    uint8_t &getVram(uint16_t address)
    {
        if (address < 0x8000 || address > 0x9FFF)
        {
            throw std::out_of_range("Address out of VRAM range");
        }
        return singleton.getVRAM()[address - 0x8000];
    };

    uint8_t &writeVram(uint16_t address, uint8_t index_value, uint8_t value)
    {
        if (address > 0x1800)
        {
            throw std::out_of_range("Address out of VRAM range");
            return;
        }
        // tile rows are two bytes.. we get the first byte with this
        uint16_t normalized = address & 0xFFFE;
        uint8_t byte1 = singleton.getVRAM()[normalized - 0x8000];
        uint8_t byte2 = singleton.getVRAM()[normalized - 0x8000 + 1];
        // these two bytes are tiles.
        // we process the tile.
        uint8_t tile_index = index_value / 16;
        uint8_t row = (index_value % 16) / 2;
        for (int i = 0; i < 8; i++)
        {
            // goes from right to left, most sig to least sig
            uint8_t mask = 1 << (7 - i);
            uint8_t bit1 = bit1 & mask;
            uint8_t bit2 = bit2 & mask;
        }

        value = switch (bit1, bit2)
        {
        case (0, 0):
            return 0; // white
        case (0, 1):
            return 1; // light gray
        case (1, 0):
            return 2; // dark gray
        case (1, 1):
            return 3; // black
        };
    };

}   
}