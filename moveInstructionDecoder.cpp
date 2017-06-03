#include "moveInstructionDecoder.hpp"

void MoveInstructionDecoder::clear(){
	Rn=0;
	im=0;
	address=0;
	word=0;
	load=false;
	memory_type=REGISTER;
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
}


void MoveInstructionDecoder::move_indirect(std::list<uint16_t> code_fetched,std::array<uint16_t,128> registres){
	clear();
	load=(bool)((code_fetched.front()>>12)&0x01);
	unsigned int Rh,Rl;
	Rh=(code_fetched.front()&0x0f00)>>8;
	Rl=(code_fetched.front()&0x00f0)>>4;
	im=code_fetched.front()&0x000f;
	code_fetched.pop_front();
	address=(registres.at(Rh)<<16)+registres.at(Rl);
	memory_type=ADDRESS;
}

void MoveInstructionDecoder::move_immediate(std::list<uint16_t> code_fetched){
	clear();
	load=(bool)((code_fetched.front()>>11)&0x01);
	im=(code_fetched.front()>>4)&0x3f;
	Rn=code_fetched.front()&0x0f;
	memory_type=REGISTER;

}

void MoveInstructionDecoder::move_Rn_offset(std::list<uint16_t> code_fetched,uint32_t PC){
	clear();
	load=(bool)((code_fetched.front()>>10)&0x01);
	im=(code_fetched.front()>>6)&0x0f;
	uint32_t uoff=(code_fetched.front()&0x003f)<<16;
	code_fetched.pop_front();
	uoff+=code_fetched.front();
	code_fetched.pop_front();
	if(uoff&0x00200000){
		uoff|=0xffc00000;
	}
	int32_t off=uoff;
	address=PC+off;
	memory_type=ADDRESS;
}

void MoveInstructionDecoder::move_immediate_offset(std::list<uint16_t> code_fetched,uint32_t PC){
	clear();
	load=(bool)((code_fetched.front()>>9)&0x01);
	im=(code_fetched.front()>>6)&0x3f;
	uint32_t uoff=(code_fetched.front()&0x0003)<<16;
	code_fetched.pop_front();
	uoff+=code_fetched.front();
	code_fetched.pop_front();
	if(uoff&0x00020000){
		uoff|=0xfffc0000;
	}
	int32_t off=uoff;
	address=PC+off;
	memory_type=ADDRESS;
}

void MoveInstructionDecoder::move_Rn_address(std::list<uint16_t> code_fetched){
	clear();
	load=(bool)((code_fetched.front()>>8)&0x01);
	im=(code_fetched.front()>>4)&0x0f;
	address=(code_fetched.front()&0x0f)<<16;
	code_fetched.pop_front();
	address+=code_fetched.front();
	memory_type=ADDRESS;
}

void MoveInstructionDecoder::move_immediate_address(std::list<uint16_t> code_fetched){
	load=(bool)((code_fetched.front()>>7)&0x01);
	im=code_fetched.front()&0x3f;
	code_fetched.pop_front();
	address=code_fetched.front()<<16;
	code_fetched.pop_front();
	address+=code_fetched.front();
	code_fetched.pop_front();
	memory_type=ADDRESS;
}

void MoveInstructionDecoder::move_word_immediate(std::list<uint16_t> code_fetched){
	im=code_fetched.front()&0x3f;
	code_fetched.pop_front();
	word=code_fetched.front();
	code_fetched.pop_front();
	memory_type=WORD;
}