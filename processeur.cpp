#include "processeur.hpp"
#include <iostream>

Processeur::Processeur()
:alu(),registres(),programm_counter(0),code_fetched(0),Ra(0),Rb(0)
{}

void Processeur::fetch(Programme prog){
	code_fetched=prog.get(programm_counter);
}

void Processeur::decode(){
	if((code_fetched&0xf000)==0){
		alu_operation();
	}

}

void Processeur::execute(){
	if((code_fetched&0xf000)==0){
		alu.update_state();
		//std::cout<<(int)alu.opcode()<<" "<<alu.resultat()<<std::endl;
	}
	programm_counter++;
}

void Processeur::alu_operation(){
	unsigned char opcode=0;
	opcode = (code_fetched>> 8)&0x0f;
	Ra =  (code_fetched>> 4)&0x0f;
	Rb = (code_fetched)&0x0f;
	alu.inputA()=registres.registre(Ra);
	alu.inputA()=registres.registre(Rb);
	alu.opcode()=opcode;
	//std::cout<<(unsigned int)opcode<<" "<<(unsigned int)Ra<<" "<<(unsigned int)Rb<<std::endl;
}

unsigned short int Processeur::codeFetched(){
	return code_fetched;
}