#pragma once

#include "MemoryInterface.hpp"
#include "Operations.hpp"
#include <cstdint>

typedef enum Flag
{
	CARRY,
	ZERO,
	IRQ_DISABLE,
	DECIMAL_MODE,
	BRK_COMMAND,
	PLACEHOLDER_BIT, //USELESS placeholder bit in status reg
	OVRFLW,			 //OVERFLOW is a reserved word in c++
	NEGATIVE
} flag_t;

typedef enum REG
{
	STATUS, // flags
	STACK,	// stack pointer
	ACCUM,	// accumulator

	// index/general registers
	IND_X,
	IND_Y
} reg_t;

typedef struct op_code_params op_code_params_t;

typedef enum op_code_t : uint8_t;

typedef struct cycleListener
{
	std::function<void()> callback;
	uint64_t targetCycle; //fires _the following_ cycle
} cycle_listener_t;

class CPU_6502 : public MemoryInterface
{
private:
	uint8_t *regs;	 // registers
	uint16_t Pc;	 // program counter
	uint64_t cycles; // TODO: implement cycle counting
	std::vector<cycle_listener_t> cycleCallbacks;

public:
	CPU_6502();

	CPU_6502(MemoryMapper *m);

	~CPU_6502();

	//TODO: figure out if these count as a cycle as well
	uint8_t getReg(reg_t reg) { return this->regs[reg]; }
	uint8_t *getRegs() { return this->regs; }
	void setReg(reg_t reg, uint8_t byte) { this->regs[reg] = byte; }

	uint16_t getPc() { return this->Pc; }
	void setPc(uint16_t pc) { this->Pc = pc; }

	uint64_t getCycles() { return this->cycles; } // for external scheduling
	//iterates the cycle count by 1 and checks for callbacks to fire based on the cycle
	void dispatchCycle()
	{
		//Each read or write on the 6502 is one cycle
		this->cycles++;
		for (uint i; i< this->cycleCallbacks.size(); i++)
		{
			cycle_listener_t c = this->cycleCallbacks[i];
			if (this-> cycles > c.targetCycle){
				c.callback();
				this-> cycleCallbacks.erase(cycleCallbacks.begin() + i);
			}
		}
	}
	uint8_t read(uint16_t address) {dispatchCycle(); return map->read(address); };
	void write(uint16_t address, char byte) {dispatchCycle(); map->write(address, byte); };

	// Derives opcode params based on opcode fetched using PC, returns via reference
	void fetch(uint8_t &, op_code_params_t &);

	// Executes op code based on pparameters passed to it as a pointer
	void execute(uint8_t, op_code_params_t);

	// allocate memory for registers and address space and initialize the program counter
	void reset(uint16_t);
};
