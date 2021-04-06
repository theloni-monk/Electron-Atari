#include <cstdint>
#include <cstdio>
#include "MemoryMapper.hpp"
//combo of TIA, Cart, and 6502 memory maps
//TODO: implement diff maps individually then combine them to atarimemmap
// spec: https://www.problemkaputt.de/2k6specs.htm#memoryandiomap
class AtariMemMap: public MemoryMapper 
{

};