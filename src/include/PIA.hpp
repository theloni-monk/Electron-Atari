#include <cstdint>
#include <cstdio>
#include "MemoryMapper.hpp"
//combo of TIA, Cart, and PIA memory maps
// spec: https://www.problemkaputt.de/2k6specs.htm#memoryandiomap

//Periferal Interface Adapter - in charge of I/O
class PIA: public MemoryMapper 
{
public:
    
    
};