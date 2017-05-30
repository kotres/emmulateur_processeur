#pragma once
#ifndef ALU_HPP
#define ALU_HPP
#include <cstdint>

enum  ALU_opcode
{
	ADD,
	ADDC,
	SUB,
	SUBC,
	RSUB,
	RSBC,
	INC,
	DEC,
	UMUL,
	SMUL,
	UDIV,
	SDIV,
	RR,
	RRC,
	RL,
	RLC,
	LRS,
	LLS,
	AND,
	OR,
	NAND,
	XOR,
	NOT,
	TCP,
	SET,
	SETB,
	CLR,
	CLRB,
	CMP,
};

class ALU{
private:
	uint16_t minputL,minputR,mresultat;
	uint8_t mcondition_reg;
	ALU_opcode mopcode;
public:
	void update_state();
	uint16_t& inputL();
	uint16_t inputL() const;
	uint16_t& inputR();
	uint16_t inputR() const;
	ALU_opcode& opcode();
	ALU_opcode opcode() const;
	uint16_t resultat() const;
	uint8_t condition_reg() const;
	uint8_t& condition_reg();
};

#endif
