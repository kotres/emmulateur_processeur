#include "instruction.hpp"

Instruction::Instruction(uint16_t opcode,uint16_t opcode_size,InstructionType type,unsigned char size)
:mopcode(opcode),mopcode_size(opcode_size),mtype(type),msize(size)
{}

bool Instruction::valIn(uint16_t val) const{
	return ((val>>(16-mopcode_size))==mopcode);
}

uint16_t Instruction::opcode() const{
	return mopcode;
}

InstructionType Instruction::type() const{
	return mtype;
}

uint16_t Instruction::opcode_size() const{
	return mopcode_size;
}

unsigned char Instruction::size() const{
	return msize;
}