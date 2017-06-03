#pragma once

#ifndef MOVE_INSTRUCTION_DECODER_HPP
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
	bool load;
	MemoryType memory_type;
	uint32_t address;
	unsigned int im,Rn;
	uint16_t word;
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
};

#define MOVE_INSTRUCTION_DECODER_HPP
#endif