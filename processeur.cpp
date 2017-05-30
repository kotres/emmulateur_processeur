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
			code_fetched.pop_front();
		break;
		case ILLEGAL:
			std::cout<<"illegal"<<std::endl;
			code_fetched.pop_front();
		break;
		case LOAD:
			std::cout<<"load"<<std::endl;
			load();
		break;
		case STORE:
			std::cout<<"store"<<std::endl;
			code_fetched.pop_front();
		break;
		case ALU_OP:
			std::cout<<"alu operation"<<std::endl;
			code_fetched.pop_front();
		break;
		case JUMP:
			std::cout<<"jump"<<std::endl;
			code_fetched.pop_front();
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


void Processeur::load(){
	if (!(code_fetched.front()&0x1000))
	{
		switch((code_fetched.front()>>10)&0x3){
			case 0:
				code_fetched.pop_front();
			break;
			case 1:
			{
				unsigned int Rn=code_fetched.front()&0x00ff;
				code_fetched.pop_front();
				registres.at(Rn)=code_fetched.front();
				code_fetched.pop_front();
				std::cout<<"word "<<std::hex<<registres.at(Rn)<<" loaded to R"<<Rn<<std::endl;
			}
			break;
			case 2:
			case 3:
			default:
				code_fetched.pop_front();
			break;
		}
	}
	else{
		code_fetched.pop_front();
	}
}