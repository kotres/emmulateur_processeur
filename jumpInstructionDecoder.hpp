#pragma once
#ifndef JUMP_INSTRUCTION_DECODER
#define JUMP_INSTRUCTION_DECODER

#include "instruction.hpp"

#include <list>

enum JumpTypes
{
	NO_TYPE,
	UNCONDITIONAL,
	CONDITIONAL,
	COMPARATIVE
};

enum  JumpCondition
{
	NO_CONDITION,
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
	unsigned int mim,mRn;
	unsigned char mcompare_operation; 
	JumpCondition mcondition;
	void clear();
	void jump_offset(std::list<uint16_t> code_fetched,uint32_t PC);
	void jump_compare_offset(std::list<uint16_t> code_fetched,uint32_t PC);
	void jump_compare(std::list<uint16_t> code_fetched);
	void jump_compare_immediate_word(std::list<uint16_t> code_fetched);
	void jump_address(std::list<uint16_t> code_fetched);
public:
	void decode(Instruction instruction, std::list<uint16_t> code_fetched,uint32_t PC);
};

#endif