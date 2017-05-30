#include "processeur.hpp"
#include <iostream>


#define TWO_WORD_MIN 0x5010
#define THREE_WORD_MIN 0x7c01

Processeur::Processeur()
:alu(),registres(),programm_counter(0),stack_pointer(0),code_fetched(),liste_instructions(),typeInstruction(NOP),
etat(FETCH),buffer(0)
{
	liste_instructions.push_back(Instruction(0x0,16,NOP));
	liste_instructions.push_back(Instruction(0x1,16,ILLEGAL));
	liste_instructions.push_back(Instruction(0x3,2,JUMP));
	liste_instructions.push_back(Instruction(0x2,2,ALU_OP));
	liste_instructions.push_back(Instruction(0x2,3,LOAD));
	liste_instructions.push_back(Instruction(0x3,3,STORE));
}

void Processeur::fetch(Programme& prog){
	std::cout<<"fetch"<<std::endl;
	if (programm_counter<=0xff)
	{
		if (code_fetched.size()==0)
		{
			std::cout<<"interrupt "<<programm_counter/2<<" happened"<<std::endl;
		}
		if(code_fetched.size()<2){
			code_fetched.push_back(prog(programm_counter));
			programm_counter++;
		}
		else{
			programm_counter=code_fetched.front()<<16;
			code_fetched.pop_front();
			programm_counter+=code_fetched.front();
			code_fetched.clear();
			std::cout<<"new programm counter: "<<std::hex<<programm_counter<<std::endl;
		}

	}
	else{
		if (code_fetched.size()<3)
		{
			code_fetched.push_back(prog(programm_counter));
			std::cout<<std::hex<<code_fetched.back()<<std::endl;
			programm_counter++;
		}
		if (code_fetched.size()>=3){
			etat=DECODE;
		}
		
	}
}


void Processeur::decode(){
	typeInstruction=ILLEGAL;
	std::cout<<"decode"<<std::endl;
	std::list<Instruction>::iterator it;
	for(it=liste_instructions.begin();it!=liste_instructions.end();it++){
		if (it->valIn(code_fetched.front()))
		{
			typeInstruction=it->type();
		}
	}
	std::cout<<"code "<<code_fetched.front()<<std::endl;
	std::cout<<"instruction "<<typeInstruction<<" trouvé"<<std::endl;
	etat=EXECUTE;
}

void Processeur::execute(Programme& prog){
	std::cout<<"execute"<<std::endl;
	uint16_t dont_care=prog(50);
	dont_care++;
	switch(typeInstruction){
		case NOP:
			std::cout<<"nop"<<std::endl;
		break;
		case ILLEGAL:
			std::cout<<"illegal"<<std::endl;
		break;
		case LOAD:
			std::cout<<"load"<<std::endl;
		break;
		case STORE:
			std::cout<<"store"<<std::endl;
		break;
		case ALU_OP:
			std::cout<<"alu operation"<<std::endl;
		break;
		case JUMP:
			std::cout<<"jump"<<std::endl;
		break;
	}
	code_fetched.pop_front();
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