#include "ALU.hpp"
#include <iostream>

uint16_t& ALU::inputL(){
	return minputL;
}

uint16_t ALU::inputL() const{
	return minputL;
}

uint16_t& ALU::inputR(){
	return minputR;
}

uint16_t ALU::inputR() const{
	return minputR;
}

ALU_opcode& ALU::opcode(){
	return mopcode;
}

ALU_opcode ALU::opcode() const{
	return mopcode;
}

uint16_t ALU::resultat() const{
	return mresultat;
}

uint8_t ALU::condition_reg() const{
	return mcondition_reg;
}

uint8_t& ALU::condition_reg(){
	return mcondition_reg;
}

void ALU::update_state(){
	uint32_t mresultat32=0;
	mresultat=0;
	switch(mopcode){
		case AND:
			mresultat=minputL&minputR;
		break;
		case OR:
			mresultat=minputL|minputR;
		break;
		case XOR:
			mresultat=(minputL^minputR);
		break;
		case NOT:
			mresultat=~minputL;
		break;
		case TCP:
			mresultat=-minputL;
		break;
		case LLS:
			mresultat=minputL<<minputR;
		break;
		case LRS:
			mresultat=minputL>>minputR;
		break;
		case UMUL:
			mresultat=(minputL&0x00ff)*(minputR&0x00ff);
		break;
		case UDIV:
			if (!minputR)
			{
				mresultat=minputL/minputR;
			}
			else{
				mresultat=0;
			}
		break;
		case ADD:
			mresultat32=minputL+minputR;
			mresultat=mresultat32;
		break;
		case SUB:
			mresultat32=minputL-minputR;
			mresultat=mresultat32;
		break;
		case INC:
			mresultat32=minputL+1;
			mresultat=mresultat32;
		break;
		case DEC:
			mresultat32=minputL-1;
			mresultat=mresultat32;
		break;
		default:
			std::cout<<"illegal alu operation"<<std::endl;
			mresultat=0;
		break;
	}
	if(mresultat32>>16!=0)
		mcondition_reg|=0x01;
	if(mresultat==0)
		mcondition_reg|=0x02;
	if((mresultat&0x8000)!=0)
		mcondition_reg|=0x04;
	if(mopcode==9&&minputR==0)
		mcondition_reg|=0x08;
}