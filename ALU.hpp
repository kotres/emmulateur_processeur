#pragma once
#ifndef ALU_HPP
#define ALU_HPP

class ALU{
private:
	unsigned short int minputS,minputD,mresultat;
	unsigned char mcondition_reg;
	unsigned char mopcode;
public:
	void update_state();
	unsigned short int& inputS();
	unsigned short int inputS() const;
	unsigned short int& inputD();
	unsigned short int inputD() const;
	unsigned char& opcode();
	unsigned char opcode() const;
	unsigned short int resultat() const;
	unsigned char condition_reg() const;
};

#endif
