#include <cstdint>
#include <cstdio>
#include "MemoryMapper.hpp"
//combo of TIA, Cart, and PIA memory maps
// spec: https://www.problemkaputt.de/2k6specs.htm#memoryandiomap

//Periferal Interface Adapter - in charge of I/O
class PIA
{
public:
    /** 
     * PIA RAM Mirrors (Step 100h,400h):
     * PIA RAM is selected by A12=0, A9=0, A7=1. Located in memory at 0080h-00FFh, 0180h-01FFh, 0480h-04FFh, 0580h-05FFh, etc. 
     * The mirror at 0180h is particulary important because it allows the CPU to access RAM for stack operations.
     * PIA I/O Mirrors (Step 2h,8h,100h,400h):
     * PIA I/O is selected by A12=0, A9=1, A7=1. 
     * Located in memory at 0280h-02FFh, 0380h-03FFh, 0680h-06FFh, 0780h-07FFh, etc. 
     * Each 80h-area contains 16 mirrors of the PIA I/O ports at XX80h, XX88h, XX90h, XX98h, etc. 
     * And, each 8h-area contains two copies of INTIM (eg. 0284h,0286h) and INSTAT (eg. 0285h,0287h).
     */
    memzone_t memZones[11] =
        {
            {0x08, 0x7F, {0x180}, 1}, //RAM     11111111  128 bytes RAM (in PIA chip) for variables and stack
            {0x0280, 1, {0x0380}, 1},  //SWCHA   11111111  Port A; input or output  (read or write)
            {0x0281, 1, {0x0381}, 1},  //SWACNT  11111111  Port A DDR, 0= input, 1=output
            {0x0282, 1, {0x0382}, 1},  //SWCHB   11111111  Port B; console switches (read only)
            {0x0283, 1, {0x0383}, 1},  //SWBCNT  11111111  Port B DDR (hardwired as input)
            {0x0284, 1, {0x0384}, 1},  //INTIM   11111111  Timer output (read only)
            {0x0285, 1, {0x0385}, 1},  //INSTAT  11......  Timer Status (read only, undocumented)
            {0x0294, 1, {0x0394}, 1},  //TIM1T   11111111  set 1 clock interval (838 nsec/interval)
            {0x0295, 1, {0x0395}, 1},  //TIM8T   11111111  set 8 5lock interval (6.7 usec/interval)
            {0x0296, 1, {0x0396}, 1},  //TIM64T  11111111  set 64 clock interval (53.6 usec/interval)
            {0x0297, 1, {0x0397}, 1},  //T1024T  11111111  set 1024 clock interval (858.2 usec/interval)
        };

    enum zoneName{
        RAM;
        SWCHA;
        SWCHB;
        SWBCNT;
        INTIM;
        INSTAT;
        TIM1T;
        TIM8T;
        TIM64T;
        TIM1024T;
    }

    PIA() {}

    uint8_t read(){
        
    }
};