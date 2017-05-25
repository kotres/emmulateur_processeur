#pragma once
#ifndef PROCESSEUR_HPP
#define PROCESSEUR_HPP

#include <list>
#include <cstdint>

#include "registres.hpp"
#include "ALU.hpp"
#include "programme.hpp"
#include "instruction.hpp"

class Processeur
{
private:
	ALU alu;
	Registres registres;
	uint32_t programm_counter;
	uint16_t code_fetched;
	std::list<Instruction>listeInstructions;
	nomInstruction instruction;
	void aluOperation();
	void jumpRelative();
	void loadRegImm();
	void loadStoreIn(Programme& prog);
	void storeRegToOffset(Programme& prog);
	void pushPop(Programme& prog);
	void asmcOffReg(Programme prog);
	void loadPCtoReg();
public:
	void fetch(Programme prog);
	void decode();
	void execute(Programme& prog);
	Processeur();
	uint16_t codeFetched();
};

#endif