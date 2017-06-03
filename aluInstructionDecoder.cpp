#include "aluInstructionDecoder.hpp"

#include <iostream>

void AluInstructionDecoder::clear(){
	mRs=0;
	mRd=0;
	misWord=false;
	mword=0;
	mopcode=OR;
}

void AluInstructionDecoder::decode(std::list<uint16_t> code_fetched){
	clear();
	std::cout<<"alu operation decoded"<<std::endl;
	mRs=(code_fetched.front()&0xf0)>>4;
	mRd=(code_fetched.front()&0x0f);
	std::cout<<"Rs: R"<<mRs<<std::endl;
	mopcode=(ALU_opcode)((code_fetched.front()&0x1f00)>>8);
	std::cout<<"opcode"<<((code_fetched.front()&0x1f00)>>8)<<std::endl;
	std::cout<<"Rd: R"<<mRd<<std::endl;
	if (code_fetched.front()&0x2000)
	{
		code_fetched.pop_front();
		misWord=true;
		mword=code_fetched.front();
	}
}

unsigned int AluInstructionDecoder::Rs() const{
	return mRs;
}
unsigned int AluInstructionDecoder::Rd() const{
	return mRd;
}
bool AluInstructionDecoder::isWord() const{
	return misWord;
}
uint16_t AluInstructionDecoder::word() const{
	return mword;
}
ALU_opcode AluInstructionDecoder::opcode() const{
	return mopcode;
}