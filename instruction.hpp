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

enum SourceDestination{
	NO_SOURCE_DEST,
	Word,
	ADDRESS,
	RN,
	OFFSET,
	INDIRECT
};

/*enum InstructionType{
	no_type,
	jump,
	alu,
	load,
	store,
};*/

class Instruction
{
private:
	uint16_t mopcode;
	uint16_t max;
	bool indirect;
	SourceDestination msource1;
	//InstructionType type;
	//AluOperation op;
public:
	Instruction(uint16_t opcode,uint16_t max,bool indirect,SourceDestination source1/*,InstructionType type,AluOperation aluOp*/);
	bool valIn(uint16_t val) const;
	uint16_t opcode() const;
	SourceDestination source1() const;
};

#endif