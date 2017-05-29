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
	FETCH1,
	FETCH2,
	FETCH3,
	DECODE,
	READ,
	EXECUTE,
	WRITE,
};

class Processeur
{
private:
	ALU alu;
	std::array<uint16_t,16> registres;
	uint32_t programm_counter;
	uint32_t stack_pointer;
	std::array<uint16_t,3> code_fetched;
	std::list<Instruction> liste_instructions;
	std::list<Instruction>::iterator instruction;

	EtatProcesseur etat;
	uint16_t buffer1,buffer2;
	void fetch1(Programme& prog);
	void fetch2(Programme& prog);
	void fetch3(Programme& prog);
	void read(Programme& prog);
	void decode();
public:
	void clock_cycle(Programme& prog);
	Processeur();
	Processeur(const Processeur& proc);
	Processeur& operator=(const Processeur& proc);
};

#endif