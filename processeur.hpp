#pragma once
#ifndef PROCESSEUR_HPP
#define PROCESSEUR_HPP

#include <list>

#include "registres.hpp"
#include "ALU.hpp"
#include "programme.hpp"
#include "instruction.hpp"

class Processeur
{
private:
	ALU alu;
	Registres registres;
	unsigned int programm_counter;
	unsigned short int code_fetched;
	std::list<Instruction>listeInstructions;
	nomInstruction instruction;
	void aluOperation();
	void jumpRelative();
	void loadRegImm();
	void loadStoreIn(Programme& prog);
	void storeRegToOffset(Programme& prog);
	void pushPop(Programme& prog);
public:
	void fetch(Programme prog);
	void decode();
	void execute(Programme& prog);
	Processeur();
	unsigned short int codeFetched();
};

#endif