#pragma once

#include "CPU.hpp"
#include "MemoryMapper.hpp"
#include "Operations.hpp"


CPU_6502::CPU_6502():MemoryInterface()
{
	this->regs = new uint8_t[5]();
	this->Pc = 0x0000;
	this->cycles = 0;
}

CPU_6502::CPU_6502(MemoryMapper* m):MemoryInterface(m)
{
	this->regs = new uint8_t[5]();
	this->Pc = 0x0000;
	this->cycles = 0;
}

CPU_6502::~CPU_6502()
{
	delete[] this->regs;
	//delete this;
}


// reset regs and addressSpace and reinitialize the program counter
void CPU_6502::reset(uint16_t PC_start = 0x60)
{
	delete[] this->regs;
	this->regs = new uint8_t[5]();

	//FIXME: reset memory mapper maybe? // maybe put reset in runtime env 
	
	this->Pc = PC_start;
}

// helper struct factory
op_code_params_t makeParams(unsigned char operand, char16_t addr, addressing_mode_t addrMode)
{
	op_code_params_t o;
	o.address = addr;
	o.operand = operand;
	o.mode = addrMode;
	
	return o;
}

// Derives opcode params based on opcode fetched using PC, returns via reference
void CPU_6502::fetch(uint8_t &op, op_code_params_t &params)
{
	uint8_t opcode = this->read(this->Pc);
	
	if(instructionNames[opcode] == FUT)
	{
		printf("fut name called in fetch");
		throw "Exception! Unimplemented OpCode";
	}

	op_code_params_t op_params;
	
	addressing_mode_t mode = instructionModes[opcode];

	uint16_t address;
	uint8_t operand;
	
	switch(mode) // addressing modes modify how the fetch operation aquires the operand of the instruction 
	{
		case Absolute:
			//piece together 16 bit address from 2 bytes
			//following op code
		{
			//curly braces to allow for variable declaration
			//inside of case statement
			uint8_t lowerByte = this->read(this->Pc + 1);
			uint8_t upperByte = this->read(this->Pc + 2);
			address = (upperByte << 8) | lowerByte;
			op_params = makeParams(0, address, mode);
		}
		break;
		case AbsoluteX:
		{
			uint8_t lowerByte = this->read(this->Pc + 1);
			uint8_t upperByte = this->read(this->Pc + 2);
			address = (upperByte << 8) | lowerByte;
			uint16_t xVal = this->regs[IND_X];
			op_params = makeParams(0, address + xVal, mode);
		}
		break;
		case AbsoluteY:
		{
			uint8_t lowerByte = this->read(this->Pc + 1);
			uint8_t upperByte = this->read(this->Pc + 2);
			address = (upperByte << 8) | lowerByte;
			uint16_t yVal =this->regs[IND_Y];
			op_params = makeParams(0, address + yVal, mode);
		}
		break;
		case Accum_mode:
			address = 0; //ADDRESS IS NOT APPLICABLE IN THIS MODE
			operand = this->regs[ACCUM];
			op_params = makeParams(operand, address, mode);
			break;
		case Immediate:
			address = this->Pc + 1;
			operand = this->read(address);
			op_params = makeParams(operand, address, mode);
			break;
		case Implied:
			address = this->Pc;
			op_params = makeParams(0, address, mode);
			break;
		case IndexedIndirect:
		{
			uint16_t immediateVal = this->read(this->Pc + 1);
			uint16_t xVal = this->regs[IND_X];
			uint8_t lowerByte = this->read(immediateVal + xVal);
			uint8_t upperByte = this->read(immediateVal + xVal + 1);
			address = (upperByte << 8) | lowerByte;
			operand = this->read(address);
			op_params = makeParams(operand, address, mode);
		}
		break;
		case Indirect:
		{
			uint8_t lowerByte = this->read(this->Pc + 1);
			uint8_t upperByte = this->read(this->Pc + 2);
			address = (upperByte << 8) | lowerByte;
			uint8_t l = this->read(address);
			uint8_t u = this->read(address + 1);
			uint16_t finalAddress = (u << 8) | l;
			op_params = makeParams(0, finalAddress, mode);
		}
		break;
		case IndirectIndexed:
		{
			uint16_t immediateVal = this->read(this->Pc + 1);
			uint8_t lowerByte = this->read(immediateVal);
			uint8_t upperByte = this->read(immediateVal + 1);
			uint16_t yVal = this->regs[IND_Y];
			address = ((upperByte << 8) | lowerByte) + yVal;
			operand = this->read(address);
			op_params = makeParams(operand, address, mode);
		}
		break;
		case Relative: //for branching within +-128 
		{
			uint16_t offset = this->read(this->Pc + 1);
			if(offset < 0x80){
				address = this->Pc + 2 + offset;
			}
			else {
				address = this->Pc + 2 + offset - 0x100;
			}
			op_params = makeParams(0, address, mode);
		}
		break;
		case ZP:
			address = 0x00FF & this->read(this->Pc + 1);
			operand = this->read(address);
			op_params = makeParams(operand, address, mode);
			break;
		case ZPX:
		{
			address = 0x00FF & this->read(this->Pc + 1);
			int8_t xVal = this->regs[IND_X];
			address += xVal;
			operand = this->read(address);
			op_params = makeParams(operand, address, mode);
		}
		break;
		case ZPY:
		{
			address = 0x00FF & this->read(this->Pc + 1);
			int8_t yVal = this->regs[IND_Y];
			address += yVal;
			operand = this->read(address);
			op_params = makeParams(operand, address, mode);
		}
		break;
	}
	
	op_params.instructionSize = instructionSizes[opcode];

	// change values passed by reference
	op = opcode;
	params = op_params;
}

// Executes op code based on parameters struct
void CPU_6502::execute(uint8_t op, op_code_params_t params)
{
	this->Pc += params.instructionSize; // go to next opcode
	
	opcode_to_func[op](this, &params);
	// TODO: count cycles
}

