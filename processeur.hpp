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
	void nop(Programme &prog);
public:
	void fetch(Programme prog);
	void decode();
	void execute(Programme &prog);
	Processeur();
	Processeur(const Processeur& proc);
	Processeur& operator=(const Processeur& proc);
};

#endif