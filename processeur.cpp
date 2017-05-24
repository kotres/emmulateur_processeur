#include "processeur.hpp"
#include <iostream>

#define RETI 0x8200

Processeur::Processeur()
:alu(),registres(),programm_counter(0),code_fetched(0),listeInstructions({Instruction(ALUoperation,0x0100,0x0fff)}),
instruction(nop),Ra(0),Rb(0)
{
	listeInstructions.push_back(Instruction(jump_conditional,0x1000,0x17ff));
	listeInstructions.push_back(Instruction(jump_relative,0x1800,0x1fff));
	listeInstructions.push_back(Instruction(load_reg_imm,0x2000,0x27ff));
	listeInstructions.push_back(Instruction(load_store_in,0x4000,0x5fff));
	listeInstructions.push_back(Instruction(Load_store_offset,0x6000,0x7fff));
	listeInstructions.push_back(Instruction(software_interrupt,0x8000,0x80ff));
	listeInstructions.push_back(Instruction(push_pop,0x8100,0x81ff));
	listeInstructions.push_back(Instruction(reti,0x8200,0x8200));
	listeInstructions.push_back(Instruction(sleep,0x8201,0x8201));
	listeInstructions.push_back(Instruction(load_PC_to_reg,0x8300,0x83ff));
	listeInstructions.push_back(Instruction(jump_indirect,0x8400,0x84ff));
	listeInstructions.push_back(Instruction(djnz,0x9000,0x9fff));
	listeInstructions.push_back(Instruction(asmc_im_Reg,0xa000,0xbfff));
	listeInstructions.push_back(Instruction(asmc_B_reg,0xc000,0xffff));

}

void Processeur::fetch(Programme prog){
	code_fetched=prog.get(programm_counter);
	programm_counter++;
}

void Processeur::decode(){
	instruction=nop;
	for(auto inst:listeInstructions){
		if(inst.valIn(code_fetched)){
			instruction=inst.Nom();
			std::cout<<instruction<<std::endl;
		}
	}
}

void Processeur::execute(){
	if((code_fetched&0xf000)==0){
		alu.update_state();
		registres(Ra)=alu.resultat();
		//std::cout<<(int)alu.opcode()<<" "<<alu.resultat()<<std::endl;
	}
}

void Processeur::alu_operation(){
	unsigned char opcode=0;
	opcode = (code_fetched>> 8)&0x0f;
	Ra =  (code_fetched>> 4)&0x0f;
	Rb = (code_fetched)&0x0f;
	alu.inputA()=registres(Ra);
	alu.inputA()=registres(Rb);
	alu.opcode()=opcode;
	std::cout<<(unsigned int)opcode<<" "<<(unsigned int)Ra<<" "<<(unsigned int)Rb<<std::endl;
}

unsigned short int Processeur::codeFetched(){
	return code_fetched;
}