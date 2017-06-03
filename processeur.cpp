#include "processeur.hpp"
#include <iostream>


#define TWO_WORD_MIN 0x5010
#define THREE_WORD_MIN 0x7c01

Processeur::Processeur()
:alu(),registres(),programm_counter(0),fetch_address(0),stack_pointer(0),code_fetched(),liste_instructions(),instruction(0x0,0,ILLEGAL,1,SPECIAL),
etat(FETCH),moveParameters(),jumpParameters()
{
	liste_instructions.push_back(Instruction(0x0,0,ILLEGAL,1,SPECIAL));
	liste_instructions.push_back(Instruction(0x0,16,NOP,1,SPECIAL));
	liste_instructions.push_back(Instruction(0x1,16,ILLEGAL,1,SPECIAL));
	liste_instructions.push_back(Instruction(0xc000,16,JUMP_ADDRESS,3,JUMP));
	liste_instructions.push_back(Instruction(0x7,3,JUMP_OFFSET,2,JUMP));
	liste_instructions.push_back(Instruction(0xd,4,JUMP_COMPARE_OFFSET,2,JUMP));
	liste_instructions.push_back(Instruction(0x19,5,JUMP_COMPARE,3,JUMP));
	liste_instructions.push_back(Instruction(0x19,5,JUMP_COMPARE_IMMEDIATE_WORD,4,JUMP));
	liste_instructions.push_back(Instruction(0x4,3,ALU_OP_WORD,2,ALU_OPERATION));
	liste_instructions.push_back(Instruction(0x5,3,ALU_OP,1,ALU_OPERATION));
	liste_instructions.push_back(Instruction(0x3,3,MOVE_INDIRECT,1,MOVE));
	liste_instructions.push_back(Instruction(0x5,4,MOVE_IMMEDIATE,1,MOVE));
	liste_instructions.push_back(Instruction(0x9,5,MOVE_RN_OFFSET,2,MOVE));
	liste_instructions.push_back(Instruction(0x11,6,MOVE_IMMEDIATE_OFFSET,2,MOVE));
	liste_instructions.push_back(Instruction(0x21,7,MOVE_RN_ADDRESS,2,MOVE));
	liste_instructions.push_back(Instruction(0x41,8,MOVE_IMMEDIATE_ADDESS,3,MOVE));
	liste_instructions.push_back(Instruction(0x81,9,MOVE_WORD_IMMEDIATE,2,MOVE));
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
	std::cout<<"decode"<<std::endl;
	switch(instruction.category()){
		case MOVE:
			moveParameters.decode(instruction,code_fetched,registres,programm_counter);
		break;
		case SPECIAL:
		break;
		case JUMP:
			jumpParameters.decode(instruction,code_fetched,programm_counter);
		break;
		case ALU_OPERATION:
		break;
		default:
		break;
	}
	code_fetched.clear();
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

void Processeur::execute(Programme &prog){
	std::cout<<"execute"<<std::endl;
	switch(instruction.category()){
		case SPECIAL:
			std::cout<<"special instruction"<<std::endl;
			programm_counter++;
		break;
		case MOVE:
		 move(prog);
		break;
		case JUMP:
			jump();
		break;
		case ALU_OPERATION:
		break;
		default:
		break;
	}
	code_fetched.clear();
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

void Processeur::move(Programme& prog){
	switch(moveParameters.memory_type()){
		case REGISTER:
		{
			if(moveParameters.load()){
				registres.at(moveParameters.Rn())=registres.at(moveParameters.im());
				std::cout<<"immediate "<<std::hex<<moveParameters.im()<<":"<<registres.at(moveParameters.im())<<
				" loaded to R"<<moveParameters.Rn()<<std::endl;
			}
			else{
				registres.at(moveParameters.im())=registres.at(moveParameters.Rn());
				std::cout<<"R"<<moveParameters.Rn()<<":"<<registres.at(moveParameters.Rn())<<
				" stored to immediate "<<moveParameters.im()<<std::endl;
			}
		}
		break;
		case WORD:
			{
				registres.at(moveParameters.im())=moveParameters.word();
				std::cout<<"word "<<std::hex<<moveParameters.word()<<" loaded to immediate"<<std::hex<<moveParameters.im()<<std::endl;
			}
		break;
		case ADDRESS:
		{
			if (moveParameters.load())
			{
				registres.at(moveParameters.im()) =prog(moveParameters.address());
				std::cout<<"word at address"<<std::hex<<moveParameters.address()<<
				":"<<std::hex<<prog(moveParameters.address())<< "loaded to immediate"<<std::hex<<moveParameters.im()<<std::endl;
			}
			else{
				prog(moveParameters.address())=registres.at(moveParameters.im());
				std::cout<<"immediate "<<std::hex<<moveParameters.im()<<":"<<registres.at(moveParameters.im())<<
				" stored to address "<<std::hex<<moveParameters.address()<<std::endl;
			}
		}
		break;
		default:
		break;
	}

	programm_counter+=instruction.size();
	code_fetched.clear();
	std::cout<<"move instruction executed"<<std::endl;
}

void Processeur::jump(){
	switch(jumpParameters.type()){
		case UNCONDITIONAL:
			programm_counter=jumpParameters.address();
			fetch_address=programm_counter;
			std::cout<<"unconditional jump to address "<<std::hex<<jumpParameters.address()<<" executed"<<std::endl;
		break;
		case COMPARATIVE:
		{
			uint16_t operandA=registres.at(jumpParameters.im());
			uint16_t operandB;
			if(jumpParameters.isWord()){
				operandB=jumpParameters.word();
			}
			else
				operandB=registres.at(jumpParameters.Rn());
			if(jump_compare_operation(jumpParameters.compare_operation(),operandA,operandB)){
				programm_counter=jumpParameters.address();
				fetch_address=programm_counter;
				std::cout<<"comparative jump executed, new PC "<<std::hex<<programm_counter<<std::endl;
			}
			else{
				programm_counter+=instruction.size();
				std::cout<<"comparative jump not executed, PC "<<std::hex<<programm_counter<<std::endl;
			}
		}
		break;
		default:
			programm_counter+=instruction.size();
		break;
	}
	std::cout<<"jump executed"<<std::endl;
	code_fetched.clear();
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