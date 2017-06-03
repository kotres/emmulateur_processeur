#include "jumpInstructionDecoder.hpp"

#include <iostream>

void JumpInstructionDecoder::decode(Instruction instruction,std::list<uint16_t> code_fetched,uint32_t PC){
	switch(instruction.type()){
		case JUMP_ADDRESS:
			jump_address(code_fetched);
		break;
		case JUMP_OFFSET:
			jump_offset(code_fetched,PC);
		case JUMP_COMPARE_OFFSET:
			jump_compare_offset(code_fetched,PC);
		break;
		case JUMP_COMPARE:
			jump_compare(code_fetched);
		break;
		case JUMP_COMPARE_IMMEDIATE_WORD:
			jump_compare_immediate_word(code_fetched);
		break;
		default:
		break;
	}
	std::cout<<"jump instruction decoded"<<std::endl;
}

void JumpInstructionDecoder::clear(){
	mtype=NO_TYPE;
	maddress=0;
	misWord=false;
	mword=0;
	mim=0,mRn=0;
	mcompare_operation=0; 
	mcondition=NO_CONDITION;
}

void JumpInstructionDecoder::jump_offset(std::list<uint16_t> code_fetched,uint32_t PC){
	clear();
	uint32_t uoff=(code_fetched.front()&0x007f)<<16;
	code_fetched.pop_front();
	uoff+=code_fetched.front();
	if(uoff&0x00400000)
		uoff|=0xff800000;
	int32_t off=uoff;
	maddress=PC+off;
	std::cout<<"decoded  offset jump, off "<<std::hex<<off<<std::endl;
	std::cout<<"address "<<std::hex<<maddress<<std::endl;
	mtype=UNCONDITIONAL;
}

void JumpInstructionDecoder::jump_compare_offset(std::list<uint16_t> code_fetched,uint32_t PC){
	clear();
	std::cout<<"comparative offset jump decoded"<<std::endl;
	mcompare_operation=(code_fetched.front()>>9)&0x03;
	mim=(code_fetched.front()>>5)&0x0f;
	mRn=(code_fetched.front()>>1)&0x0f;
	uint32_t uoff=0x0;
	if(code_fetched.front()&0x01)
		uoff=0xffff0000;
	code_fetched.pop_front();
	uoff+=code_fetched.front();
	code_fetched.pop_front();
	int32_t off=uoff;
	maddress=PC+off;
	std::cout<<"R"<<mim<<":"<<" compared to R"<<mRn<<std::endl;
	std::cout<<"compare operation"<<(int)mcompare_operation<<std::endl;
	std::cout<<"offset "<<std::hex<<off<<std::endl;
	std::cout<<"adress "<<std::hex<<maddress<<std::endl;
	mtype= COMPARATIVE;
}

void JumpInstructionDecoder::jump_compare(std::list<uint16_t> code_fetched){
	clear();
	std::cout<<"comparative address jump decoded"<<std::endl;
	mcompare_operation=(code_fetched.front()>>8)&0x03;
	mim=(code_fetched.front()>>4)&0x0f;
	mRn=(code_fetched.front()&0x0f);
	code_fetched.pop_front();
	maddress=code_fetched.front()<<16;
	code_fetched.pop_front();
	maddress+=code_fetched.front();
	code_fetched.pop_front();

	std::cout<<"R"<<mim<<":"<<" compared to R"<<std::endl;
	std::cout<<"compare "<<(int)mcompare_operation<<std::endl;
	std::cout<<"address "<<std::hex<<maddress<<std::endl;
	mtype= COMPARATIVE;
}

void JumpInstructionDecoder::jump_compare_immediate_word(std::list<uint16_t> code_fetched){
	clear();
	std::cout<<"comparative address jump with immediate decoded"<<std::endl;
	mcompare_operation=(code_fetched.front()>>7)&0x03;
	mim=(code_fetched.front()&0x03f);
	code_fetched.pop_front();
	maddress=code_fetched.front()<<16;
	code_fetched.pop_front();
	maddress+=code_fetched.front();
	code_fetched.pop_front();
	mword=code_fetched.front();
	code_fetched.pop_front();

	std::cout<<"immediate "<<mim<<" compared to word "<<std::hex<<mword<<std::endl;
	std::cout<<"compare "<<(int)mcompare_operation<<std::endl;
	mtype= COMPARATIVE;
	misWord=true;
}

void JumpInstructionDecoder::jump_address(std::list<uint16_t> code_fetched){
	clear();
	code_fetched.pop_front();
	maddress=code_fetched.front()<<16;
	code_fetched.pop_front();
	maddress+= code_fetched.front();
	code_fetched.clear();
	std::cout<<"unconditional jump to address "<<std::hex<<maddress<<" decoded"<<std::endl;
	mtype=UNCONDITIONAL;
}

JumpTypes JumpInstructionDecoder::type() const{
	return mtype;
}
uint32_t JumpInstructionDecoder::address() const{
	return maddress;
}
uint16_t JumpInstructionDecoder::word() const{
	return mword;
}
bool JumpInstructionDecoder::isWord() const{
	return misWord;
}
unsigned int JumpInstructionDecoder::im() const{
	return mim;
}
unsigned int JumpInstructionDecoder::Rn() const{
	return mRn;
}
unsigned char JumpInstructionDecoder::compare_operation() const{
	return mcompare_operation;
}
JumpCondition JumpInstructionDecoder::condition(){
	return mcondition;
}