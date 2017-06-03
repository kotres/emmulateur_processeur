#pragma once
#ifndef ALU_INSTRUCION_DECODER
#define ALU_INSTRUCION_DECODER
#include "ALU.hpp"
#include "instruction.hpp"

#include <list>

class AluInstructionDecoder
{

	unsigned int mRs,mRd;
	bool misWord;
	uint16_t mword;
	ALU_opcode mopcode;
	void clear();
public:
	void decode(std::list<uint16_t> code_fetched);
	unsigned int Rs() const;
	unsigned int Rd() const;
	bool isWord() const;
	uint16_t word() const;
	ALU_opcode opcode() const;
};

#endif