#pragma once
#ifndef PROCESSEUR_HPP
#define PROCESSEUR_HPP

#include <list>
#include <cstdint>
#include <array>

#include "ALU.hpp"
#include "programme.hpp"
#include "instruction.hpp"
#include "moveInstructionDecoder.hpp"

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
	Instruction instruction;
	EtatProcesseur etat;
	MoveInstructionDecoder moveParameters;
	void fetch(Programme& prog);
	void decode();
	void execute();
	void move();
	void alu_operation();
	void alu_operation_word();
	void decodeOpcode();
	void jump_address();
	void jump_offset();
	bool jump_compare_operation(uint8_t condition,uint16_t operandA,uint16_t operandB);
	void jump_compare_offset();
	void jump_compare();
	void jump_compare_immediate_word();
public:
	void clock_cycle(Programme& prog);
	Processeur();
};

#endif