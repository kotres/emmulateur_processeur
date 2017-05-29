#include "processeur.hpp"
#include <iostream>


#define TWO_WORD_MIN 0x5010
#define THREE_WORD_MIN 0x7c01

Processeur::Processeur()
:alu(),registres(),programm_counter(0),stack_pointer(0),code_fetched(),liste_instructions(),instruction(nullptr),
etat(FETCH1),buffer1(0),buffer2(0)
{
	liste_instructions.push_back(Instruction(0x0,0x0,false,NO_SOURCE_DEST));
	liste_instructions.push_back(Instruction(0x1,0x1,false,NO_SOURCE_DEST));
	liste_instructions.push_back(Instruction(0x0c00,0x0cff,false,RN));
	liste_instructions.push_back(Instruction(0x5010,0x501f,false,Word));
	liste_instructions.push_back(Instruction(0x8390,0x839f,false,ADDRESS));
}

void Processeur::fetch1(Programme& prog){
	std::cout<<"fetch1"<<std::endl;
	if (programm_counter<0xff)
	{
		std::cout<<"interrupt "<<programm_counter/2<<" happened"<<std::endl;
		code_fetched.at(0)=prog(programm_counter);
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

void Processeur::fetch2(Programme& prog){
	std::cout<<"fetch2"<<std::endl;
	if (programm_counter<0xff)
	{
		code_fetched.at(1)=prog(programm_counter);
		programm_counter=(code_fetched.at(0)<<16)+code_fetched.at(1);
		std::cout<<"fetch2: newPC="<<std::hex<<programm_counter<<std::endl;
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

void Processeur::fetch3(Programme& prog){
	std::cout<<"fetch3"<<std::endl;
	code_fetched.at(2)=prog(programm_counter);
	std::cout<<"three word instruction fetched:\n"<<std::hex<<programm_counter-2<<":"<<std::hex<<code_fetched.at(0)<<std::endl;
	std::cout<<std::hex<<programm_counter-1<<":"<<std::hex<<code_fetched.at(1)<<std::endl;
	std::cout<<std::hex<<programm_counter<<":"<<std::hex<<code_fetched.at(2)<<std::endl;
	etat=DECODE;
}

void Processeur::decode(){
	std::cout<<"decode"<<std::endl;
	instruction=liste_instructions.begin();
	instruction++;
	std::list<Instruction>::iterator it;
	for(it=liste_instructions.begin();it!=liste_instructions.end();it++){
		if (it->valIn(code_fetched.at(0)))
		{
			instruction=it;
		}
	}
	std::cout<<"instruction "<<std::hex<<instruction->opcode()<<" trouvé"<<std::endl;
	etat=READ;
}

Processeur::Processeur(const Processeur& proc)
:alu(proc.alu),registres(),programm_counter(0),stack_pointer(0),code_fetched(),liste_instructions(),instruction(nullptr),
etat(FETCH1),buffer1(0),buffer2(0)
{}

Processeur& Processeur::operator=(const Processeur& proc){
	this->alu=proc.alu;
	return *this;
}

void Processeur::read(Programme& prog){

	switch(instruction->source1()){
		unsigned int Rs;
		case NO_SOURCE_DEST:
			buffer1=0;
		break;
		case Word:
			buffer1=code_fetched.at(1);
		break;
		case ADDRESS:
			buffer1=prog((code_fetched.at(1)<<16)+code_fetched.at(2));
		break;
		case RN:
			Rs=(code_fetched.at(0)&0xf0)>>4;
			buffer1=registres.at(Rs);
		break;
		case OFFSET:
		break;
		case INDIRECT:
		break;
		default:
		break;
	}
	std::cout<<"source1: "<<std::hex<<buffer1<<std::endl;
	etat=EXECUTE;
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
			decode();
		break;
		case READ:
			read(prog);
		break;
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