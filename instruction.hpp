#pragma once

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

enum nomInstruction
{
	nop,
	
};

class Instruction
{
private:
	nomInstruction nom;
	unsigned short int min;
	unsigned short int max;
public:
	Instruction(enum nomInstruction nom,unsigned short int min,unsigned short int max);
	bool valIn(unsigned short int val);
	nomInstruction Nom();

};

#endif