#pragma once

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdint>
#include "programme.hpp"


/*enum AluOperation{
	no_op,
	add,
	sub
};*/

/*enum SourceDestination{
	NO_SOURCE_DEST,
	Word,
	ADDRESS,
	RN,
	OFFSET,
	INDIRECT
};*/

enum InstructionType{
	NOP,
	ILLEGAL,
	JUMP,
	ALU_OP,
	LOAD,
	STORE,
};

class Instruction
{
private:
	uint16_t mopcode;
	uint16_t mopcode_size;
	InstructionType mtype;
	//AluOperation op;
public:
	Instruction(uint16_t opcode,uint16_t opcode_size,InstructionType type);
	bool valIn(uint16_t val) const;
	uint16_t opcode() const;
	InstructionType type() const;
};

#endif