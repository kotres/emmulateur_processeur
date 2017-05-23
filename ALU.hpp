#pragma once
#ifndef ALU_HPP
#define ALU_HPP

class ALU{
private:
	unsigned short int minputA,minputB,mresultat;
	unsigned char mcondition_reg;
	unsigned char mopcode;
public:
	void update_state();
	unsigned short int& inputA();
	unsigned short int inputA() const;
	unsigned short int& inputB();
	unsigned short int inputB() const;
	unsigned char& opcode();
	unsigned char opcode() const;
	unsigned short int resultat() const;
	unsigned char condition_reg() const;
};

#endif
