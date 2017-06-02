#pragma once
#ifndef PROCESSEUR_HPP
#define PROCESSEUR_HPP

#include <list>
#include <cstdint>
#include <array>

#include "ALU.hpp"
#include "programme.hpp"
#include "instruction.hpp"

class Instruction;

enum EtatProcesseur{
	FETCH,
	DECODE,
	EXECUTE
};

class Processeur
{
private:
	ALU alu;
	std::array<uint16_t,128> registres;
	uint32_t programm_counter;
	uint32_t fetch_address;
	uint32_t stack_pointer;
	std::list<uint16_t> code_fetched;
	std::list<Instruction> liste_instructions;
	InstructionType typeInstruction;
	EtatProcesseur etat;
	void fetch(Programme& prog);
	void decode();
	void execute(Programme& prog);
	void loadStore(Programme& prog);
	void alu_operation();
	void jump();
	void jump_offset();
	bool jump_compare_operation(uint8_t condition,uint16_t operandA,uint16_t operandB);
	void jump_compare_offset();
	void jump_compare();
	void jump_compare_immediate_word();
	void move_indirect(Programme &prog, bool load);
	void move_immediate(bool load);
	void move_Rn_offset(Programme &prog, bool load);
	void move_immediate_offset(Programme &prog, bool load);
	void move_Rn_address(Programme &prog, bool load);
	void move_immediate_address(Programme &prog, bool load);
	void move_word_immediate();
public:
	void clock_cycle(Programme& prog);
	Processeur();
};

#endif