#pragma once

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

enum nomInstruction
{
	nop,
	ALUoperation,
	jump_conditional,
	jump_relative,
	load_reg_imm,
	load_store_in,
	Load_store_offset,
	software_interrupt,
	push_pop,
	reti,
	sleep,
	load_PC_to_reg,
	jump_indirect,
	djnz,
	asmc_im_Reg,
	asmc_B_reg,
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