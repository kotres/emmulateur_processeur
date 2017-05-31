#include "processeur.hpp"
#include <iostream>


#define TWO_WORD_MIN 0x5010
#define THREE_WORD_MIN 0x7c01

Processeur::Processeur()
:alu(),registres(),programm_counter(0),stack_pointer(0),code_fetched(),liste_instructions(),typeInstruction(NOP),
etat(FETCH)
{
	liste_instructions.push_back(Instruction(0x0,16,NOP));
	liste_instructions.push_back(Instruction(0x1,16,ILLEGAL));
	liste_instructions.push_back(Instruction(0x3,2,JUMP));
	liste_instructions.push_back(Instruction(0x2,2,ALU_OP));
	liste_instructions.push_back(Instruction(0x2,3,LOAD));
	liste_instructions.push_back(Instruction(0x3,3,STORE));
}

void Processeur::fetch(Programme& prog){
	std::cout<<"fetch PC "<<std::hex<<programm_counter<<std::endl;
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
			load(prog);
		break;
		case STORE:
			std::cout<<"store"<<std::endl;
			code_fetched.pop_front();
		break;
		case ALU_OP:
			alu_operation();
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
	if(!code_fetched.front()&0x2000){
		alu.inputR()=registres.at(Rd);
		std::cout<<"inputR: R"<<Rd<<":"<<std::hex<<registres.at(Rd)<<std::endl;
	}
	else{
		code_fetched.pop_front();
		alu.inputR()=code_fetched.front();
		std::cout<<"inputR: word "<<std::hex<<code_fetched.front()<<std::endl;
	}
	code_fetched.pop_front();
	alu.update_state();
	registres.at(Rd)=alu.resultat();
	std::cout<<"result "<<std::hex<<alu.resultat()<<" stored in R"<<Rd<<std::endl;
}

void Processeur::load(Programme prog){
	if (!(code_fetched.front()&0x1000))
	{
		switch((code_fetched.front()>>10)&0x3){
			case 0:
			{
				unsigned int Rs=(code_fetched.front()&0x00f0)>>4;
				unsigned int Rd=code_fetched.front()&0x000f;
				registres.at(Rd)=registres.at(Rs);
				code_fetched.pop_front();
				std::cout<<"R"<<Rs<<":"<<registres.at(Rs)<<" loaded to R"<<Rd<<std::endl;
			}
			break;
			case 1:
			{
				unsigned int Rn=code_fetched.front()&0x000f;
				code_fetched.pop_front();
				registres.at(Rn)=code_fetched.front();
				code_fetched.pop_front();
				std::cout<<"word "<<std::hex<<registres.at(Rn)<<" loaded to R"<<Rn<<std::endl;
			}
			break;
			case 2:
			{
				unsigned int Rn=code_fetched.front()&0x000f;
				code_fetched.pop_front();
				uint32_t addr=code_fetched.front()<<16;
				code_fetched.pop_front();
				addr+=code_fetched.front();
				code_fetched.pop_front();
				registres.at(Rn)=prog(addr);
				std::cout<<"word at address "<<std::hex<<addr<<":"<<std::hex<<prog(addr)<<" loaded to R"<<Rn<<std::endl;
			}
			break;
			case 3:
			{
				unsigned int Rn=(code_fetched.front()&0x00f0)>>4;
				uint32_t uoff=(code_fetched.front()&0x000f)<<16;
				code_fetched.pop_front();
				uoff+=code_fetched.front();
				code_fetched.pop_front();
				if(uoff&0x00080000){
					uoff|=0xfff00000;
				}
				int32_t off=uoff;
				registres.at(Rn)=prog(programm_counter-3+off);
				std::cout<<"offset "<<std::hex<<off<<std::endl;
				std::cout<<"R"<<Rn<<" recieved word from address "<<std::hex<<programm_counter-3+off<<std::endl;
			}
			break;
			default:
				code_fetched.pop_front();
			break;
		}
	}
	else{
		unsigned int Rh,Rl,Rd;
		Rh=(code_fetched.front()&0x0f00)>>8;
		Rl=(code_fetched.front()&0x00f0)>>4;
		Rd=code_fetched.front()&0x000f;
		code_fetched.pop_front();
		uint32_t addr=(registres.at(Rh)<<16)+registres.at(Rl);
		registres.at(Rd)=prog(addr);
		std::cout<<"indirect, word at address "<<std::hex<<addr<<":"<<std::hex<<prog(addr)<<" loaded to R"<<Rd<<std::endl;
	}
}