#pragma once

#ifndef MOVE_INSTRUCTION_DECODER_HPP

#define MOVE_INSTRUCTION_DECODER_HPP
#include "instruction.hpp"
#include <list>
#include <array>

enum MemoryType{
	REGISTER,
	ADDRESS,
	WORD
};

class MoveInstructionDecoder
{
	bool mload;
	MemoryType mmemory_type;
	uint32_t maddress;
	unsigned int mim,mRn;
	uint16_t mword;
	void clear();
	void move_indirect(std::list<uint16_t> code_fetched,std::array<uint16_t,128> registres);
	void move_immediate(std::list<uint16_t> code_fetched);
	void move_Rn_offset(std::list<uint16_t> code_fetched,uint32_t PC);
	void move_immediate_offset(std::list<uint16_t> code_fetched,uint32_t PC);
	void move_Rn_address(std::list<uint16_t> code_fetched);
	void move_immediate_address(std::list<uint16_t> code_fetched);
	void move_word_immediate(std::list<uint16_t> code_fetched);
public:
	void decode(Instruction instruction,std::list<uint16_t> code_fetched,std::array<uint16_t,128> registres,uint32_t PC);
	bool load() const;
	MemoryType memory_type() const;
	uint32_t address() const;
	unsigned int im() const;
	unsigned int Rn() const;
	uint16_t word() const;
};


#endif