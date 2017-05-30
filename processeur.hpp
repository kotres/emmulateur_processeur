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
	std::array<uint16_t,16> registres;
	uint32_t programm_counter;
	uint32_t stack_pointer;
	std::list<uint16_t> code_fetched;
	std::list<Instruction> liste_instructions;
	InstructionType typeInstruction;
	EtatProcesseur etat;

	void fetch(Programme& prog);
	void decode();
	void execute(Programme& prog);
	void load(Programme prog);
public:
	void clock_cycle(Programme& prog);
	Processeur();
};

#endif