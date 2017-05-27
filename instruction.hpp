#pragma once

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdint>
#include "processeur.hpp"
#include "programme.hpp"

class Processeur;

typedef void (Processeur::*procfunction)(Programme& programme);

class Instruction
{
private:
	uint16_t min;
	uint16_t mopcode;
	uint16_t max;
	procfunction func;
public:
	Instruction(uint16_t min,uint16_t opcode,uint16_t max,procfunction fun);
	bool valIn(uint16_t val) const;
	uint16_t opcode() const;
	void execute(Processeur* proc,Programme& programme);
};

#endif