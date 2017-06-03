#include "processeur.hpp"
#include <iostream>


#define TWO_WORD_MIN 0x5010
#define THREE_WORD_MIN 0x7c01

Processeur::Processeur()
:alu(),registres(),programm_counter(0),fetch_address(0),stack_pointer(0),code_fetched(),liste_instructions(),instruction(0x0,0,ILLEGAL,1),
etat(FETCH)
{
	liste_instructions.push_back(Instruction(0x0,0,ILLEGAL,1));
	liste_instructions.push_back(Instruction(0x0,16,NOP,1));
	liste_instructions.push_back(Instruction(0x1,16,ILLEGAL,1));
	liste_instructions.push_back(Instruction(0xc000,16,JUMP,3));
	liste_instructions.push_back(Instruction(0x7,3,JUMP_OFFSET,2));
	liste_instructions.push_back(Instruction(0xd,4,JUMP_COMPARE_OFFSET,2));
	liste_instructions.push_back(Instruction(0x19,5,JUMP_COMPARE,3));
	liste_instructions.push_back(Instruction(0x19,5,JUMP_COMPARE_IMMEDIATE_WORD,4));
	liste_instructions.push_back(Instruction(0x4,3,ALU_OP_WORD,2));
	liste_instructions.push_back(Instruction(0x5,3,ALU_OP,1));
	liste_instructions.push_back(Instruction(0x3,3,MOVE_INDIRECT,1));
	liste_instructions.push_back(Instruction(0x5,4,MOVE_IMMEDIATE,1));
	liste_instructions.push_back(Instruction(0x9,5,MOVE_RN_OFFSET,2));
	liste_instructions.push_back(Instruction(0x11,6,MOVE_IMMEDIATE_OFFSET,2));
	liste_instructions.push_back(Instruction(0x21,7,MOVE_RN_ADDRESS,2));
	liste_instructions.push_back(Instruction(0x41,8,MOVE_IMMEDIATE_ADDESS,3));
	liste_instructions.push_back(Instruction(0x81,9,MOVE_WORD_IMMEDIATE,2));
}


void Processeur::fetch(Programme& prog){
	std::cout<<"fetch at address "<<std::hex<<fetch_address<<std::endl;
	if (fetch_address<=0xff)
	{
		if (code_fetched.size()==0)
		{
			std::cout<<"interrupt "<<programm_counter/2<<" happened"<<std::endl;
		}
		if(code_fetched.size()<2){
			code_fetched.push_back(prog(fetch_address));
			fetch_address++;
		}
		else{
			programm_counter=code_fetched.front()<<16;
			code_fetched.pop_front();
			programm_counter+=code_fetched.front();
			code_fetched.clear();
			std::cout<<"new programm counter: "<<std::hex<<programm_counter<<std::endl;
			fetch_address=programm_counter;
		}

	}
	else{
		if (code_fetched.size()==0)
		{
			code_fetched.push_back(prog(fetch_address));
			std::cout<<std::hex<<code_fetched.back()<<std::endl;
			++fetch_address;
			decodeOpcode();
			if(instruction.size()==1)
				etat=DECODE;
		}
		else{
			code_fetched.push_back(prog(fetch_address));
			fetch_address++;
			if(instruction.size()<=code_fetched.size())
				etat=DECODE;
		}
	}
}


void Processeur::decode(){
	//TODO: faire la fonction decode
	std::cout<<"decode"<<std::endl;
	etat=EXECUTE;
}

void Processeur::decodeOpcode(){
	instruction= liste_instructions.front();
	std::list<Instruction>::iterator it,it_final;
	it=liste_instructions.begin();
	it_final=liste_instructions.begin();
	for(it=++it;it!=liste_instructions.end();it++){
		if (it->valIn(code_fetched.front()))
		{
			if(it->opcode_size()>it_final->opcode_size())
				it_final=it;
		}
	}
	instruction=*it_final;
	std::cout<<"code "<<code_fetched.front()<<std::endl;
	std::cout<<"instruction "<<instruction.type()<<" trouvé"<<std::endl;
}

void Processeur::execute(Programme& prog){
	std::cout<<"execute"<<std::endl;
	switch(instruction.type()){
		case NOP:
			std::cout<<"nop"<<std::endl;
			code_fetched.pop_front();
			++programm_counter;
		break;
		case ILLEGAL:
			std::cout<<"illegal"<<std::endl;
			code_fetched.pop_front();
			++programm_counter;
		break;
		case MOVE_INDIRECT:
			move_indirect(prog);
		break;
		case MOVE_IMMEDIATE:
			move_immediate();
		break;
		case MOVE_RN_OFFSET:
			move_Rn_offset(prog);
		break;
		case MOVE_IMMEDIATE_OFFSET:
			move_immediate_offset(prog);
		break;
		case MOVE_RN_ADDRESS:
			move_Rn_address(prog);
		break;
		case MOVE_IMMEDIATE_ADDESS:
			move_immediate_address(prog);
		break;
		case MOVE_WORD_IMMEDIATE:
			move_word_immediate();
		break;
		case ALU_OP:
			alu_operation();
		break;
		case ALU_OP_WORD:
			alu_operation_word();
		break;
		case JUMP:
			jump();
		break;
		case JUMP_OFFSET:
			std::cout<<"jump offset"<<std::endl;
			jump_offset();
		break;
		case JUMP_COMPARE_OFFSET:
			jump_compare_offset();
		break;
		case JUMP_COMPARE:
			jump_offset();
		case JUMP_COMPARE_IMMEDIATE_WORD:
			jump_compare_immediate_word();
		break;
		default:
		break;
	}
	etat=FETCH;
}

void Processeur::clock_cycle(Programme& prog){
	switch(etat){
		case FETCH:
			fetch(prog);
		break;
		case DECODE:
			decode();
		break;
		case EXECUTE:
			execute(prog);
		break;
		default:
		break;
	}
}

void Processeur::alu_operation(){
	std::cout<<"alu operation"<<std::endl;
	unsigned int Rs,Rd;
	Rs=(code_fetched.front()&0xf0)>>4;
	Rd=(code_fetched.front()&0x0f);
	alu.inputL()=registres.at(Rs);
	std::cout<<"Rs: R"<<Rs<<":"<<std::hex<<registres.at(Rs)<<std::endl;
	ALU_opcode op=(ALU_opcode)((code_fetched.front()&0x1f00)>>8);
	alu.opcode()=op;
	std::cout<<"opcode"<<((code_fetched.front()&0x1f00)>>8)<<std::endl;
	alu.inputR()=registres.at(Rd);
	std::cout<<"inputR: R"<<Rd<<":"<<std::hex<<registres.at(Rd)<<std::endl;
	code_fetched.pop_front();
	alu.update_state();
	registres.at(Rd)=alu.resultat();
	std::cout<<"result "<<std::hex<<alu.resultat()<<" stored in R"<<Rd<<std::endl;
	++programm_counter;
}

void Processeur::alu_operation_word(){
	std::cout<<"alu operation with word"<<std::endl;
	unsigned int Rs,Rd;
	Rs=(code_fetched.front()&0xf0)>>4;
	Rd=(code_fetched.front()&0x0f);
	alu.inputL()=registres.at(Rs);
	std::cout<<"Rs: R"<<Rs<<":"<<std::hex<<registres.at(Rs)<<std::endl;
	ALU_opcode op=(ALU_opcode)((code_fetched.front()&0x1f00)>>8);
	alu.opcode()=op;
	std::cout<<"opcode"<<((code_fetched.front()&0x1f00)>>8)<<std::endl;

	code_fetched.pop_front();
	alu.inputR()=code_fetched.front();
	std::cout<<"inputR: word "<<std::hex<<code_fetched.front()<<std::endl;

	code_fetched.pop_front();
	alu.update_state();
	registres.at(Rd)=alu.resultat();
	std::cout<<"result "<<std::hex<<alu.resultat()<<" stored in R"<<Rd<<std::endl;
	programm_counter+=2;
}

void Processeur::move_indirect(Programme &prog){
	bool load=(bool)((code_fetched.front()>>12)&0x01);
	unsigned int Rh,Rl,Rd;
	Rh=(code_fetched.front()&0x0f00)>>8;
	Rl=(code_fetched.front()&0x00f0)>>4;
	Rd=code_fetched.front()&0x000f;
	code_fetched.pop_front();
	uint32_t addr=(registres.at(Rh)<<16)+registres.at(Rl);
	if(load){
		registres.at(Rd)=prog(addr);
		std::cout<<"indirect, word at address "<<std::hex<<addr<<":"<<std::hex<<prog(addr)<<" loaded to R"<<Rd<<std::endl;
	}
	else{
		prog(addr)=registres.at(Rd);
		std::cout<<"R"<<Rd<<":"<<std::hex<<registres.at(Rd)<<" stored to indirect address "<<std::hex<<addr<<std::endl;
	}
	++programm_counter;
}

void Processeur::move_immediate(){
	bool load=(bool)((code_fetched.front()>>11)&0x01);
	unsigned int Rn,im;
	im=(code_fetched.front()>>4)&0x3f;
	Rn=code_fetched.front()&0x0f;
	code_fetched.pop_front();
	if(load){
		registres.at(Rn)=registres.at(im);
		std::cout<<"word at immediate "<<std::hex<<im<<":"<<std::hex<<registres.at(im)<<" loaded to R"<<Rn<<std::endl;
	}
	else{
		registres.at(im)=registres.at(Rn);
		std::cout<<"R"<<Rn<<":"<<std::hex<<registres.at(Rn)<<" stored to im "<<std::hex<<im<<std::endl;
	}
	++programm_counter;
}

void Processeur::move_Rn_offset(Programme &prog){
	bool load=(bool)((code_fetched.front()>>10)&0x01);
	unsigned int Rn=(code_fetched.front()>>6)&0x0f;
	uint32_t uoff=(code_fetched.front()&0x003f)<<16;
	code_fetched.pop_front();
	uoff+=code_fetched.front();
	code_fetched.pop_front();
	if(uoff&0x00200000){
		uoff|=0xffc00000;
	}
	int32_t off=uoff;
	std::cout<<"offset "<<std::hex<<off<<std::endl;
	if(load){
		registres.at(Rn)=prog(programm_counter+off);
		std::cout<<"word at address "<<std::hex<<programm_counter-3+off
		<<":"<<std::hex<<prog(programm_counter+off)<<" loaded to R"<<Rn<<std::endl;
	}
	else{
		prog(programm_counter+off)=registres.at(Rn);
		std::cout<<"R"<<Rn<<":"<<std::hex<<registres.at(Rn)
		<<"stored to address"<<std::hex<<programm_counter-3+off<<std::endl;
		if(off==1){
			code_fetched.front()=registres.at(Rn);
		}
	}
	programm_counter+=2;
}

void Processeur::move_immediate_offset(Programme &prog){
	bool load=(bool)((code_fetched.front()>>9)&0x01);
	unsigned int imm=(code_fetched.front()>>6)&0x3f;
	uint32_t uoff=(code_fetched.front()&0x0003)<<16;
	code_fetched.pop_front();
	uoff+=code_fetched.front();
	code_fetched.pop_front();
	if(uoff&0x00020000){
		uoff|=0xfffc0000;
	}
	int32_t off=uoff;
	std::cout<<"offset "<<std::hex<<off<<std::endl;
	if(load){
		registres.at(imm)=prog(programm_counter+off);
		std::cout<<"word at address "<<std::hex<<programm_counter+off<<
		":"<<std::hex<<prog(programm_counter+off)<<" loaded to immediate "<<std::hex<<imm<<std::endl;
	}
	else{
		prog(programm_counter+off)=registres.at(imm);
		std::cout<<"immediate"<<std::hex<<imm<<":"<<std::hex<<registres.at(imm)<<" stored to address "
		<<std::hex<<programm_counter+off<<std::endl;
		if(off==1){
			code_fetched.front()=registres.at(imm);
		}
	}
	programm_counter+=2;
}

void Processeur::move_Rn_address(Programme &prog){
	bool load=(bool)((code_fetched.front()>>8)&0x01);
	unsigned int Rn=(code_fetched.front()>>4)&0x0f;
	uint32_t address=(code_fetched.front()&0x0f)<<16;
	code_fetched.pop_front();
	address+=code_fetched.front();
	code_fetched.pop_front();
	if(load){
		registres.at(Rn)=prog(address);
		std::cout<<"word at address"<<std::hex<<address<<":"<<std::hex<<prog(address)<<
		" loaded to register R"<<Rn<<std::endl;
	}
	else{
		prog(address)=registres.at(Rn);
		std::cout<<"register R"<<Rn<<":"<<std::hex<<registres.at(Rn)<<" stored to address "<<
		std::hex<<address<<std::endl;
	}
	programm_counter+=2;
}

void Processeur::move_immediate_address(Programme &prog){
	bool load=(bool)((code_fetched.front()>>7)&0x01);
	unsigned int im=code_fetched.front()&0x3f;
	code_fetched.pop_front();
	uint32_t address=code_fetched.front()<<16;
	code_fetched.pop_front();
	address+=code_fetched.front();
	code_fetched.pop_front();
	if(load){
		registres.at(im)=prog(address);
		std::cout<<"word at address"<<std::hex<<address<<":"<<std::hex<<prog(address)<<
		" loaded to immediate "<<im<<std::endl;
	}
	else{
		prog(address)=registres.at(im);
		std::cout<<"immediate "<<im<<":"<<std::hex<<registres.at(im)<<" stored to address "<<
		std::hex<<address<<std::endl;
	}
	programm_counter+=3;
}

void Processeur::move_word_immediate(){
	unsigned int im=code_fetched.front()&0x3f;
	code_fetched.pop_front();
	uint16_t word=code_fetched.front();
	code_fetched.pop_front();
	registres.at(im)=word;
	std::cout<<"word "<<std::hex<<word<<" loaded to immediate "<<std::hex<<im<<std::endl;
	programm_counter+=2;
}

void Processeur::jump_offset(){
	uint32_t uoff=(code_fetched.front()&0x1fff)<<16;
	code_fetched.pop_front();
	uoff+=code_fetched.front();
	if(uoff&0x10000000)
		uoff|=0xf0000000;
	int32_t off=uoff;
	programm_counter+=off;
	fetch_address=programm_counter;
	std::cout<<"jump offset "<<std::hex<<off<<std::endl;
	std::cout<<"PC "<<std::hex<<programm_counter<<std::endl;
	code_fetched.clear();
}

void Processeur::jump_compare_offset(){
	uint8_t op=(code_fetched.front()>>9)&0x03;
	unsigned int Ra=(code_fetched.front()>>5)&0x0f;
	unsigned int Rb=(code_fetched.front()>>1)&0x0f;
	uint32_t uoff=0x0;
	if(code_fetched.front()&0x01)
		uoff=0xffff0000;
	code_fetched.pop_front();
	uoff+=code_fetched.front();
	code_fetched.pop_front();
	int32_t off=uoff;

	std::cout<<"R"<<Ra<<":"<<std::hex<<registres.at(Ra)
	<<" compared to R"<<Rb<<":"<<std::hex<<registres.at(Rb)<<std::endl;
	std::cout<<"compare "<<(int)op<<std::endl;


	if(jump_compare_operation(op,registres.at(Ra),registres.at(Rb))){
		programm_counter+=off;
		fetch_address=programm_counter;
		std::cout<<"jump occured, offset "<<std::hex<<off<<"\nnew PC "<<std::hex<<programm_counter<<std::endl;
		code_fetched.clear();
	}
	else{
		programm_counter+=2;
		std::cout<<"no jump occured"<<std::endl;
	}
}

void Processeur::jump_compare(){
	uint8_t op=(code_fetched.front()>>8)&0x03;
	unsigned int Ra=(code_fetched.front()>>4)&0x0f;
	unsigned int Rb=(code_fetched.front()&0x0f);
	code_fetched.pop_front();
	uint32_t address=code_fetched.front()<<16;
	code_fetched.pop_front();
	address+=code_fetched.front();
	code_fetched.pop_front();

	std::cout<<"R"<<Ra<<":"<<std::hex<<registres.at(Ra)
	<<" compared to R"<<Rb<<":"<<std::hex<<registres.at(Rb)<<std::endl;
	std::cout<<"compare "<<(int)op<<std::endl;


	if(jump_compare_operation(op,registres.at(Ra),registres.at(Rb))){
		programm_counter=address;
		fetch_address=programm_counter;
		std::cout<<"jump occured, address "<<std::hex<<address<<"\nnew PC "<<std::hex<<programm_counter<<std::endl;
		code_fetched.clear();
	}
	else{
		programm_counter+=3;
		std::cout<<"no jump occured"<<std::endl;
	}
}

void Processeur::jump_compare_immediate_word(){
	uint8_t op=(code_fetched.front()>>7)&0x03;
	unsigned int im=(code_fetched.front()&0x03f);
	code_fetched.pop_front();
	uint32_t address=code_fetched.front()<<16;
	code_fetched.pop_front();
	address+=code_fetched.front();
	code_fetched.pop_front();
	uint16_t word=code_fetched.front();
	code_fetched.pop_front();

	std::cout<<"immediate "<<im<<":"<<std::hex<<registres.at(im)
	<<" compared to word "<<std::hex<<word<<std::endl;
	std::cout<<"compare "<<(int)op<<std::endl;

	if(jump_compare_operation(op,registres.at(im),word)){
		programm_counter=address;
		fetch_address=programm_counter;
		std::cout<<"jump occured, address "<<std::hex<<address<<"\nnew PC "<<std::hex<<programm_counter<<std::endl;
		code_fetched.clear();
	}
	else{
		programm_counter+=4;
		std::cout<<"no jump occured"<<std::endl;
	}
}

bool Processeur::jump_compare_operation(uint8_t condition,uint16_t operandA,uint16_t operandB){
	bool ret=false;
	switch(condition){
		case 0:
			ret=(operandA==operandB);
		break;
		case 1:
			ret=(operandA!=operandB);
		break;
		case 2:
			ret=(operandA>operandB);
		break;
		case 3:
			ret=(operandA<operandB);
		break;
		case 4:
			ret=(operandA>=operandB);
		break;
		case 5:
			ret=(operandA<=operandB);
		break;
		case 6:
			ret=(operandA==0&&operandB==0);
		case 7:
			ret=(operandA+operandB>0xffff);
		default:
		break;
	}
	return ret;
}


void Processeur::jump(){
	code_fetched.pop_front();
	uint32_t address=code_fetched.front()<<16;
	code_fetched.pop_front();
	address+= code_fetched.front();
	code_fetched.clear();
	programm_counter=address;
	fetch_address=programm_counter;
	std::cout<<"unconditional jump to address "<<std::hex<<address<<std::endl;
}