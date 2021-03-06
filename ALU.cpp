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
		case NAND:
			mresultat=~(minputL&minputR);
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
		case SET:
			mresultat=0xffff;
		break;
		case SETB:
			mresultat=minputL|(0x01<<minputR);
		break;
		case CLR:
			mresultat=0x0;
		break;
		case CLRB:
			mresultat=minputL&(~(0x01<<minputR));
		break;
		case LLS:
			mresultat=(minputL<<minputR);
		break;
		case LRS:
			mresultat=(minputL>>minputR);
		break;
		case RL:
		{
			uint16_t right_part=(minputL>>(16-minputR%16));
			mresultat=(minputL<<(minputR%16));
			mresultat+=right_part;
		}
		break;
		case RLC:
		{
			uint64_t iL_with_carry=minputL+((mcondition_reg&0x01)<<16);
			iL_with_carry=(iL_with_carry<<(minputR%17));
			mresultat=iL_with_carry+(iL_with_carry>>17);
			mcondition_reg&=0xfe;
			mcondition_reg+=((iL_with_carry>>16)&0x01);
		}
		break;
		case RR:
		{
			uint16_t left_part=(minputL<<(16-minputR%16));
			mresultat=(minputL>>minputR%16);
			mresultat+=left_part;
		}
		break;
		// case RRC:
		// {
		// 	uint64_t iL_with_carry=minputL<<48+((mcondition_reg&0x01)<<47);
		// 	iL_with_carry=(iL_with_carry>>(minputR%17));
		// 	mresultat=(iL_with_carry>>48)+(iL_with_carry>>17);
		// 	mcondition_reg&=0xfe;
		// 	mcondition_reg+=((iL_with_carry>>16)&0x01);
		// }
		break;
		case UMUL:
			mresultat=(minputL&0x00ff)*(minputR&0x00ff);
		break;
		case SMUL:
		{
			int8_t left=minputL;
			int8_t right=minputR;
			mresultat=left*right;
		}
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
		case SDIV:
			if (!minputR)
			{
				int16_t left=minputL;
				int16_t right=minputR;
				mresultat=left/right;
			}
			else{
				mresultat=0;
			}
		break;
		case ADD:
			mresultat32=minputL+minputR;
			mresultat=mresultat32;
		break;
		case ADDC:
			mresultat32=minputL+minputR+(mcondition_reg&0x01);
			mresultat=mresultat32;
		break;
		case CMP:
		case SUB:
			mresultat32=minputL-minputR;
			mresultat=mresultat32;
		break;
		case SUBC:
			mresultat32=minputL-minputR-(mcondition_reg&0x01);
			mresultat=mresultat32;
		break;
		case RSUB:
			mresultat32=minputR-minputL;
			mresultat=mresultat32;
		break;
		case RSBC:
			mresultat32=minputR-minputL-(mcondition_reg&0x01);
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
	if(mresultat&0x8000)
		mcondition_reg|=0x04;
	if((mopcode==UDIV||mopcode==SDIV)&&minputR==0)
		mcondition_reg|=0x08;
	if(mopcode==CMP)
		mresultat=minputL;
}
