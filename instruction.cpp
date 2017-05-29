#include "instruction.hpp"

Instruction::Instruction(uint16_t opcode,uint16_t max,bool indirect,SourceDestination source1/*,InstructionType type,AluOperation aluOp*/)
:mopcode(opcode),max(max),indirect(indirect),msource1(source1)//,type(type),op(aluOp)
{}

bool Instruction::valIn(uint16_t val) const{
	return (val>=mopcode&&val<=max);
}

uint16_t Instruction::opcode() const{
	return mopcode;
}

SourceDestination Instruction::source1() const{
	return msource1;
}