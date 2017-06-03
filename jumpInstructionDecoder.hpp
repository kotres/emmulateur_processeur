#pragma once
#ifndef JUMP_INSTRUCTION_DECODER
#define JUMP_INSTRUCTION_DECODER

#include <list>

enum JumpTypes
{
	DIRECT,
	CONDITIONAL,
	COMPARATIVE;
};

enum  JumpCondition
{
	CARRY_SET,
	CARRY_CLEAR,
	POSITIVE,
	NOT_POSITIVE,
	NEGATIVE,
	NOT_NEGATIVE,
	ZERO,
	NOT_ZERO,
	OVERFLOW,
	NOT_OVERFLOW,
	BIT_SET,
	BIT_CLEAR,
};

class JumpInstructionDecoder
{
	JumpTypes mtype;
	uint32_t maddress;
	bool misWord;
	uint16_t mword;
	unsigned char compare_operation; 
	JumpCondition mcondition;
public:
	decode(std::list<uint16_t> code_fetched);
};

#endif