#pragma once

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdint>
#include "programme.hpp"


enum InstructionCategory{
	SPECIAL,
	MOVE,
	JUMP,
	ALU_OPERATION,
};

enum InstructionType{
	NOP,
	ILLEGAL,
	JUMP_ADDRESS,
	JUMP_OFFSET,
	JUMP_COMPARE_OFFSET,
	JUMP_COMPARE,
	JUMP_COMPARE_IMMEDIATE_WORD,
	ALU_OP,
	ALU_OP_WORD,
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
	unsigned char msize;
	InstructionCategory mcategory;
public:
	Instruction(uint16_t opcode,uint16_t opcode_size,InstructionType type,unsigned char size,InstructionCategory category);
	bool valIn(uint16_t val) const;
	uint16_t opcode() const;
	InstructionType type() const;
	uint16_t opcode_size() const;
	unsigned char size() const;
	InstructionCategory category() const;
};

#endif