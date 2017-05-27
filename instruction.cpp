#include "instruction.hpp"

Instruction::Instruction(uint16_t min,uint16_t opcode,uint16_t max,procfunction fun)
:min(min),mopcode(opcode),max(max),func(fun)
{}

bool Instruction::valIn(uint16_t val) const{
	return (val>=min&&val<=max);
}

uint16_t Instruction::opcode() const{
	return mopcode;
}

void Instruction::execute(Processeur* proc,Programme& programme){
	(proc->*(this->func)) (programme);
}