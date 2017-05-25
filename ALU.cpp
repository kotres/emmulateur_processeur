#include "ALU.hpp"

unsigned short int& ALU::inputS(){
	return minputS;
}

unsigned short int ALU::inputS() const{
	return minputS;
}

unsigned short int& ALU::inputD(){
	return minputD;
}

unsigned short int ALU::inputD() const{
	return minputD;
}

unsigned char& ALU::opcode(){
	return mopcode;
}

unsigned char ALU::opcode() const{
	return mopcode;
}

unsigned short int ALU::resultat() const{
	return mresultat;
}

unsigned char ALU::condition_reg() const{
	return mcondition_reg;
}

void ALU::update_state(){
	unsigned int mresultat32=0;
	mresultat=0;
	switch(mopcode){
		case 1:
			mresultat=minputS&minputD;
		break;
		case 2:
			mresultat=minputS|minputD;
		break;
		case 3:
			mresultat=(minputS^minputD);
		break;
		case 4:
			mresultat=~minputS;
		break;
		case 5:
			mresultat=-minputS;
		break;
		case 6:
			mresultat=minputS<<minputD;
		break;
		case 7:
			mresultat=minputS>>minputD;
		break;
		case 8:
			mresultat=(minputS&0x00ff)*(minputD&0x00ff);
		break;
		case 9:
			if (minputD!=0)
			{
				mresultat=minputS/minputD;
			}
			else{
				mresultat=0;
			}
		break;
		case 10:
			mresultat32=minputS+minputD;
			mresultat=mresultat32;
		break;
		case 11:
			mresultat32=minputS-minputD;
			mresultat=mresultat32;
		break;
		case 12:
			mresultat32=++minputS;
			mresultat=mresultat32;
		break;
		case 13:
			mresultat32=--minputS;
			mresultat=mresultat32;
		break;
		default:
			mresultat=0;
		break;
	}
	mcondition_reg=0x00;
	if(mopcode>=10&&mopcode<=13){
		if(mresultat32<<16!=0)
			mcondition_reg|=0x01;
	}
	if(mresultat==0)
		mcondition_reg|=0x02;
	if((mresultat&0x8000)!=0)
		mcondition_reg|=0x04;
	if(mopcode==9&&minputD==0)
		mcondition_reg|=0x08;
}