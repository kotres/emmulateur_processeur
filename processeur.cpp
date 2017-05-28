#include "processeur.hpp"
#include <iostream>


#define TWO_WORD_MIN 0x5000
#define THREE_WORD_MIN 0x8b00

Processeur::Processeur()
:alu(),registres(),programm_counter(0),stack_pointer(0),code_fetched(),liste_instructions(),instruction(nullptr),
etat(FETCH1),newPC(0)
{
	liste_instructions.push_back(Instruction(0,0,0,&Processeur::nop,true));
	liste_instructions.push_back(Instruction(1,1,1,&Processeur::undefined,true));
	liste_instructions.push_back(Instruction(0x5000,0x5000,0x500f,&Processeur::loadWordToRegister,true));
}

void Processeur::fetch1(Programme prog){
	std::cout<<"fetch1"<<std::endl;
	if (programm_counter<0xff)
	{
		std::cout<<"interrupt "<<programm_counter/2<<" happened"<<std::endl;
		newPC = (prog(programm_counter)<<16);
		std::cout<<"fetch1: newPC="<<std::hex<<newPC<<std::endl;
		etat=FETCH2;
		programm_counter++;
	}
	else{
		code_fetched.at(0)=prog(programm_counter);
		if(code_fetched.at(0)<TWO_WORD_MIN){
			std::cout<<"one word instruction fetched: PC="<<std::hex<<programm_counter<<
			":"<<std::hex<<code_fetched.at(0)<<std::endl;
			etat=DECODE;
		}
		else{
			programm_counter++;
			etat=FETCH2;
		}
	}
}

void Processeur::fetch2(Programme prog){
	std::cout<<"fetch2"<<std::endl;
	if (programm_counter<0xff)
	{
		newPC += prog(programm_counter);
		std::cout<<"fetch2: newPC="<<std::hex<<newPC<<std::endl;
		programm_counter=newPC;
		etat=FETCH1;
	}
	else{
		code_fetched.at(1)=prog(programm_counter);
		if(code_fetched.at(0)<THREE_WORD_MIN){
			std::cout<<"two word instruction fetched:\n"<<std::hex<<programm_counter-1<<":"<<std::hex<<code_fetched.at(0)<<std::endl;
			std::cout<<std::hex<<programm_counter<<":"<<std::hex<<code_fetched.at(1)<<std::endl;
			etat=DECODE;
		}
		else{
			programm_counter++;
			etat=FETCH3;
		}
	}
}

void Processeur::fetch3(Programme prog){
	std::cout<<"fetch3"<<std::endl;
	code_fetched.at(2)=prog(programm_counter);
	std::cout<<"three word instruction fetched:\n"<<std::hex<<programm_counter-2<<":"<<std::hex<<code_fetched.at(0)<<std::endl;
	std::cout<<std::hex<<programm_counter-1<<":"<<std::hex<<code_fetched.at(1)<<std::endl;
	std::cout<<std::hex<<programm_counter<<":"<<std::hex<<code_fetched.at(2)<<std::endl;
	etat=DECODE;
}

void Processeur::decode(){
	instruction=liste_instructions.begin();
	instruction++;
	std::list<Instruction>::iterator it;
	for(it=liste_instructions.begin();it!=liste_instructions.end();it++){
		if (it->valIn(code_fetched.at(0)))
		{
			instruction=it;
		}
	}
}

void Processeur::execute(Programme &prog){
	instruction->execute(this,prog);
	if(instruction->increasePC())
		programm_counter++;
}

void Processeur::nop(Programme &prog){
	uint16_t dont_care=prog(0);
	dont_care++;
	std::cout<<"nop executé"<<std::endl;
}

void Processeur::undefined(Programme &prog){
	uint16_t dont_care=prog(0);
	dont_care++;
	std::cout<<"undifined operation"<<std::endl;
}

void Processeur::loadWordToRegister(Programme &prog){
	uint16_t dont_care=prog(0);
	dont_care++;
	unsigned int Rn=code_fetched.at(0)&0x0f;
	registres.at(Rn)=code_fetched.at(1);
	std::cout<<"load word to register"<<std::endl;
	std::cout<<"R"<<Rn<<"="<<std::hex<<registres.at(Rn)<<std::endl;
}



Processeur::Processeur(const Processeur& proc)
:alu(proc.alu),registres(),programm_counter(0),stack_pointer(0),code_fetched(),liste_instructions(),instruction(nullptr),
etat(FETCH1),newPC(0)
{}

Processeur& Processeur::operator=(const Processeur& proc){
	this->alu=proc.alu;
	return *this;
}

void Processeur::clock_cycle(Programme& prog){
	switch(etat){
		case FETCH1:
			fetch1(prog);
		break;
		case FETCH2:
			fetch2(prog);
		break;
		case FETCH3:
			fetch3(prog);
		break;
		case DECODE:
		//break;
		case READ:
		//break;
		case EXECUTE:
		//break;
		case WRITE:
		//break;
		default:
		programm_counter++;
		etat=FETCH1;
		break;
	}
}