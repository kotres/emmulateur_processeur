#include "processeur.hpp"
#include <iostream>


#define TWO_WORD_MIN 0x5000
#define THREE_WORD_MIN 0x8b00

Processeur::Processeur()
:alu(),registres(),programm_counter(0),stack_pointer(0),code_fetched()
{}

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
			std::cout<<std::hex<<programm_counter<<":"<<std::hex<<code_fetched.at(0)<<std::endl;
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

}

void Processeur::execute(){
	programm_counter++;
}