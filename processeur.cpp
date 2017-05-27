#include "processeur.hpp"
#include <iostream>


#define TWO_WORD_MIN 0x5000
#define THREE_WORD_MIN 0x8b00

Processeur::Processeur()
:alu(),registres(),programm_counter(0),stack_pointer(0),code_fetched(),liste_instructions(),instruction(nullptr)
{
	liste_instructions.push_back(Instruction(0,0,0,&Processeur::nop,true));
	liste_instructions.push_back(Instruction(1,1,1,&Processeur::undefined,true));
	liste_instructions.push_back(Instruction(0x5000,0x5000,0x500f,&Processeur::loadWordToRegister,true));
}

void Processeur::fetch(Programme prog){
	if (programm_counter<0xff)
	{
		std::cout<<"interrupt "<<programm_counter/2<<" happened"<<std::endl;
		uint32_t int_vector = (prog(programm_counter)<<16)+prog(++stack_pointer);
		std::cout<<"interrupt vector: "<<std::hex<<int_vector<<std::endl;
		programm_counter=int_vector;
	}
	code_fetched.at(0)=prog(programm_counter);
	if (code_fetched.at(0)<TWO_WORD_MIN)
	{
		std::cout<<"1 word code fetched:"<<std::endl
		<<std::hex<<programm_counter<<":"<<std::hex<<code_fetched.at(0)<<std::endl;
	}
	else{
		programm_counter++;
		code_fetched.at(1)=prog(programm_counter);
		if (code_fetched.at(0)<THREE_WORD_MIN)
		{
			std::cout<<"2 word code fetched:"<<std::endl
			<<std::hex<<(programm_counter-1)<<":"<<std::hex<<code_fetched.at(0)<<std::endl;
			std::cout<<std::hex<<programm_counter<<":"<<std::hex<<code_fetched.at(1)<<std::endl;
		}
		else{
			programm_counter++;
			code_fetched.at(2)=prog(programm_counter);
			std::cout<<"3 word code fetched:"<<std::endl
			<<std::hex<<(programm_counter-2)<<":"<<std::hex<<code_fetched.at(0)<<std::endl;
			std::cout<<std::hex<<(programm_counter-1)<<":"<<std::hex<<code_fetched.at(1)<<std::endl;
			std::cout<<std::hex<<programm_counter<<":"<<std::hex<<code_fetched.at(2)<<std::endl;
		}
	}
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
:alu(proc.alu),registres(),programm_counter(0),stack_pointer(0),code_fetched(),liste_instructions(),instruction(nullptr)
{}

Processeur& Processeur::operator=(const Processeur& proc){
	this->alu=proc.alu;
	return *this;
}