#include "instruction.hpp"

Instruction::Instruction(enum nomInstruction nom,unsigned short int min,unsigned short int max)
:nom(nom),min(min),max(max)
{}

bool Instruction::valIn(unsigned short int val){
	return (val>=min&&val<=max);
}

nomInstruction Instruction::Nom(){
	return nom;
}