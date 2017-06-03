#include "moveInstructionDecoder.hpp"
#include <iostream>

void MoveInstructionDecoder::clear(){
	mRn=0;
	mim=0;
	maddress=0;
	mword=0;
	mload=false;
	mmemory_type=REGISTER;
}

void MoveInstructionDecoder::decode(Instruction instruction,std::list<uint16_t> code_fetched,std::array<uint16_t,128> registres,uint32_t PC){
	switch(instruction.type()){
		case MOVE_INDIRECT:
			move_indirect(code_fetched,registres);
		break;
		case MOVE_IMMEDIATE:
			move_immediate(code_fetched);
		break;
		case MOVE_RN_OFFSET:
			move_Rn_offset(code_fetched,PC);
		break;
		case MOVE_IMMEDIATE_OFFSET:
			move_immediate_offset(code_fetched,PC);
		break;
		case MOVE_RN_ADDRESS:
			move_Rn_address(code_fetched);
		break;
		case MOVE_IMMEDIATE_ADDESS:
			move_immediate_address(code_fetched);
		break;
		case MOVE_WORD_IMMEDIATE:
			move_word_immediate(code_fetched);
		break;
		default:
		break;
	}
	std::cout<<"move instruction decoded"<<std::endl;
}


void MoveInstructionDecoder::move_indirect(std::list<uint16_t> code_fetched,std::array<uint16_t,128> registres){
	clear();
	std::cout<<"move indirect"<<std::endl;
	mload=(bool)((code_fetched.front()>>12)&0x01);
	if (mload)
	{
		std::cout<<"load"<<std::endl;
	}
	else
		std::cout<<"store"<<std::endl;
	unsigned int Rh,Rl;
	Rh=(code_fetched.front()&0x0f00)>>8;
	Rl=(code_fetched.front()&0x00f0)>>4;
	std::cout<<"Rh "<<Rh<<":"<<std::hex<<registres.at(Rh)<<" Rl "<<Rl<<":"<<std::hex<<registres.at(Rl)<<std::endl;
	mim=code_fetched.front()&0x000f;
	code_fetched.pop_front();
	maddress=(registres.at(Rh)<<16)+registres.at(Rl);
	std::cout<<"address "<<std::hex<<maddress<<std::endl;
	mmemory_type=ADDRESS;
}

void MoveInstructionDecoder::move_immediate(std::list<uint16_t> code_fetched){
	clear();
	mload=(bool)((code_fetched.front()>>11)&0x01);
	mim=(code_fetched.front()>>4)&0x3f;
	mRn=code_fetched.front()&0x0f;
	mmemory_type=REGISTER;

}

void MoveInstructionDecoder::move_Rn_offset(std::list<uint16_t> code_fetched,uint32_t PC){
	clear();
	mload=(bool)((code_fetched.front()>>10)&0x01);
	mim=(code_fetched.front()>>6)&0x0f;
	uint32_t uoff=(code_fetched.front()&0x003f)<<16;
	code_fetched.pop_front();
	uoff+=code_fetched.front();
	code_fetched.pop_front();
	if(uoff&0x00200000){
		uoff|=0xffc00000;
	}
	int32_t off=uoff;
	maddress=PC+off;
	mmemory_type=ADDRESS;
}

void MoveInstructionDecoder::move_immediate_offset(std::list<uint16_t> code_fetched,uint32_t PC){
	clear();
	mload=(bool)((code_fetched.front()>>9)&0x01);
	mim=(code_fetched.front()>>6)&0x3f;
	uint32_t uoff=(code_fetched.front()&0x0003)<<16;
	code_fetched.pop_front();
	uoff+=code_fetched.front();
	code_fetched.pop_front();
	if(uoff&0x00020000){
		uoff|=0xfffc0000;
	}
	int32_t off=uoff;
	maddress=PC+off;
	mmemory_type=ADDRESS;
}

void MoveInstructionDecoder::move_Rn_address(std::list<uint16_t> code_fetched){
	clear();
	mload=(bool)((code_fetched.front()>>8)&0x01);
	mim=(code_fetched.front()>>4)&0x0f;
	maddress=(code_fetched.front()&0x0f)<<16;
	code_fetched.pop_front();
	maddress+=code_fetched.front();
	mmemory_type=ADDRESS;
}

void MoveInstructionDecoder::move_immediate_address(std::list<uint16_t> code_fetched){
	mload=(bool)((code_fetched.front()>>7)&0x01);
	mim=code_fetched.front()&0x3f;
	code_fetched.pop_front();
	maddress=code_fetched.front()<<16;
	code_fetched.pop_front();
	maddress+=code_fetched.front();
	code_fetched.pop_front();
	mmemory_type=ADDRESS;
}

void MoveInstructionDecoder::move_word_immediate(std::list<uint16_t> code_fetched){
	mim=code_fetched.front()&0x3f;
	code_fetched.pop_front();
	mword=code_fetched.front();
	code_fetched.pop_front();
	mmemory_type=WORD;
	std::cout<<"load word "<<mword<<" to immediate "<<std::hex<<mim<<" decoded"<<std::endl;
}

bool MoveInstructionDecoder::load() const{
	return mload;
}

MemoryType MoveInstructionDecoder::memory_type() const{
	return mmemory_type;
}

uint32_t MoveInstructionDecoder::address() const{
	return maddress;
}

unsigned int MoveInstructionDecoder::im() const{
	return mim;
}

unsigned int MoveInstructionDecoder::Rn() const{
	return mRn;
}

uint16_t MoveInstructionDecoder::word() const{
	return mword;
}