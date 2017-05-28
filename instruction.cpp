#include "instruction.hpp"

Instruction::Instruction(uint16_t opcode,uint16_t max,bool indirect,SourceDestination sd,InstructionType type,AluOperation aluOp)
:mopcode(opcode),max(max),indirect(indirect),sd(sd),type(type),op(aluOp)
{}

bool Instruction::valIn(uint16_t val) const{
	return (val>=mopcode&&val<=max);
}

uint16_t Instruction::opcode() const{
	return mopcode;
}
