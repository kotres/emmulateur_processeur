#pragma once

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

enum nomInstruction
{
	nop,
	ALU_operation,
	jump_conditional,
	jump_relative,
	load_reg_imm,
	load_store_in,
	store_reg_to_offset,
	software_interrupt,
	push_pop,
	reti,
	sleep,
	load_PC_to_reg,
	jump_indirect,
	djnz,
	asmc_im_Reg,
	asmc_off_reg,
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