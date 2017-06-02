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
	JUMP_OFFSET,
	JUMP_COMPARE_OFFSET,
	JUMP_COMPARE,
	JUMP_COMPARE_IMMEDIATE_WORD,
	ALU_OP,
	MOVE_INDIRECT,
	MOVE_IMMEDIATE,
	MOVE_RN_OFFSET,
	MOVE_IMMEDIATE_OFFSET,
	MOVE_RN_ADDRESS,
	MOVE_IMMEDIATE_ADDESS,
	MOVE_WORD_IMMEDIATE,
	
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
	uint16_t opcode_size() const;
};

#endif