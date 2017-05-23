#include "ALU.hpp"

unsigned short int& ALU::inputA(){
	return minputA;
}

unsigned short int ALU::inputA() const{
	return minputA;
}

unsigned short int& ALU::inputB(){
	return minputB;
}

unsigned short int ALU::inputB() const{
	return minputB;
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
	switch(mopcode){
		case 0:
			mresultat=minputA&minputB;
		break;
		case 1:
			mresultat=minputA|minputB;
		break;
		case 2:
			mresultat=minputA^minputB;
		break;
		case 3:
			mresultat=~minputA;
		break;
		case 4:
			mresultat=-minputA;
		break;
		case 5:
			mresultat=minputA<<minputB;
		break;
		case 6:
			mresultat=minputA>>minputB;
		break;
		case 7:
			mresultat=(minputA&0x00ff)*(minputB&0x00ff);
		break;
		case 8:
			if (minputB!=0)
			{
				mresultat=minputA/minputB;
			}
			else{
				mresultat=0;
			}
		break;
		case 9:
			mresultat32=((unsigned int)minputA)+((unsigned int)minputB);
			mresultat=minputA+minputB;
		break;
		case 10:
			mresultat32=((unsigned int)minputA)-((unsigned int)minputB);
			mresultat=minputA-minputB;
		break;
		default:
			mresultat=0;
		break;
	}
	mcondition_reg=0x00;
	if(mopcode==9||mopcode==10){
		if(mresultat32<<24!=0)
			mcondition_reg|=0x01;
	}
	if(mresultat==0)
		mcondition_reg|=0x02;
	if((mresultat&0x8000)!=0)
		mcondition_reg|=0x04;
	if(mopcode==8&&minputB==0)
		mcondition_reg|=0x08;
}