#include "processeur.hpp"
#include <iostream>


#define TWO_WORD_MIN 0x5010
#define THREE_WORD_MIN 0x7c01

Processeur::Processeur()
:alu(),registres(),programm_counter(0),fetch_address(0),stack_pointer(0),code_fetched(),liste_instructions(),typeInstruction(NOP),
etat(FETCH)
{
	liste_instructions.push_back(Instruction(0x0,0,ILLEGAL));
	liste_instructions.push_back(Instruction(0x0,16,NOP));
	liste_instructions.push_back(Instruction(0x1,16,ILLEGAL));
	liste_instructions.push_back(Instruction(0x3,2,JUMP));
	liste_instructions.push_back(Instruction(0x2,2,ALU_OP));
	liste_instructions.push_back(Instruction(0x1,2,MOVE));
}

void Processeur::fetch(Programme& prog){
	std::cout<<"fetch at address "<<std::hex<<programm_counter+code_fetched.size()<<std::endl;
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
		if (code_fetched.size()<3)
		{
			code_fetched.push_back(prog(fetch_address));
			std::cout<<std::hex<<code_fetched.back()<<std::endl;
			++fetch_address;
		}
		if (code_fetched.size()>=3){
			etat=DECODE;
		}
		
	}
}


void Processeur::decode(){
	typeInstruction=ILLEGAL;
	std::cout<<"decode"<<std::endl;
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
	typeInstruction=it_final->type();
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
			++programm_counter;
		break;
		case ILLEGAL:
			std::cout<<"illegal"<<std::endl;
			code_fetched.pop_front();
			++programm_counter;
		break;
		case MOVE:
			std::cout<<"move"<<std::endl;
			loadStore(prog);
		break;
		case ALU_OP:
			alu_operation();
		break;
		case JUMP:
			std::cout<<"jump"<<std::endl;
			jump();
			++programm_counter;
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
		++programm_counter;
	}
	code_fetched.pop_front();
	alu.update_state();
	registres.at(Rd)=alu.resultat();
	std::cout<<"result "<<std::hex<<alu.resultat()<<" stored in R"<<Rd<<std::endl;
	++programm_counter;
}

void Processeur::move_indirect(Programme &prog, bool load){
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

void Processeur::move_immediate(bool load){
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

void Processeur::move_Rn_offset(Programme &prog, bool load){
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

void Processeur::move_immediate_offset(Programme &prog, bool load){
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

void Processeur::move_Rn_address(Programme &prog, bool load){
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

void Processeur::move_immediate_address(Programme &prog, bool load){
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

void Processeur::loadStore(Programme &prog){
	bool load=(bool)((code_fetched.front()>>13)&0x01);
	if((code_fetched.front()>>12)&0x01){
		move_indirect(prog,load);
	}
	else if((code_fetched.front()>>11)&0x01){
		move_immediate(load);
	}
	else if((code_fetched.front()>>10)&0x01){
		move_Rn_offset(prog,load);
	}
	else if((code_fetched.front()>>9)&0x01){
		move_immediate_offset(prog,load);
	}
	else if((code_fetched.front()>>8)&0x01){
		move_Rn_address(prog,load);
	}
	else if((code_fetched.front()>>7)&0x01){
		move_immediate_address(prog,load);
	}
	else if((code_fetched.front()&0xdfff)<0x4080){
		move_word_immediate();
	}
	else{
		std::cout<<"move function failed"<<std::endl;
		code_fetched.pop_front();
		programm_counter++;
	}
}

void Processeur::jump(){
	switch((code_fetched.front()&0x3c00)>>10){
		case 0:
		{
			switch((code_fetched.front()&0x0300)>>8){
				case 0:
				{
					uint32_t uoff=(code_fetched.front()&0x00ff);
					if(uoff&0x00000080){
					uoff|=0xffffff00;
					}
					int32_t off=uoff;
					programm_counter=programm_counter+off;
					code_fetched.clear();
					std::cout<<"small jump PC="<<std::hex<<programm_counter<<std::endl;
				}
				break;
				default:
				break;
			}
		}
		break;
		default:
		break;
	}
}