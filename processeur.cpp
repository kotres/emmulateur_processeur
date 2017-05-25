#include "processeur.hpp"
#include <iostream>


#define RETI 0x8200

Processeur::Processeur()
:alu(),registres(),programm_counter(0),code_fetched(0),listeInstructions({Instruction(ALU_operation,0x0100,0x0fff)}),
instruction(nop)
{
	listeInstructions.push_back(Instruction(jump_conditional,0x1000,0x17ff));
	listeInstructions.push_back(Instruction(jump_relative,0x1800,0x1fff));
	listeInstructions.push_back(Instruction(load_reg_imm,0x2000,0x27ff));
	listeInstructions.push_back(Instruction(load_store_in,0x4000,0x5fff));
	listeInstructions.push_back(Instruction(store_reg_to_offset,0x6000,0x6fff));
	listeInstructions.push_back(Instruction(software_interrupt,0x8000,0x80ff));
	listeInstructions.push_back(Instruction(push_pop,0x8100,0x81ff));
	listeInstructions.push_back(Instruction(reti,0x8200,0x8200));
	listeInstructions.push_back(Instruction(sleep,0x8201,0x8201));
	listeInstructions.push_back(Instruction(load_PC_to_reg,0x8300,0x83ff));
	listeInstructions.push_back(Instruction(jump_indirect,0x8400,0x84ff));
	listeInstructions.push_back(Instruction(djnz,0x9000,0x9fff));
	listeInstructions.push_back(Instruction(asmc_im_Reg,0xa000,0xbfff));
	listeInstructions.push_back(Instruction(asmc_off_reg,0xc000,0xffff));

}

void Processeur::fetch(Programme prog){
	code_fetched=prog.get(programm_counter);
}

void Processeur::decode(){
	instruction=nop;
	for(auto inst:listeInstructions){
		if(inst.valIn(code_fetched)){
			instruction=inst.Nom();
			//std::cout<<instruction<<std::endl;
		}
	}
}

void Processeur::execute(Programme& prog){
	std::cout<<"PC: "<<std::hex<<programm_counter<<", code fetched: "<<std::hex<<code_fetched<<":"<<std::endl;
	switch(instruction){
		case nop:
		break;
		case ALU_operation:
			aluOperation();
		break;
		case jump_conditional:
		break;
		case jump_relative:
			jumpRelative();
		break;
		case load_reg_imm:
			loadRegImm();
		break;
		case load_store_in:
			loadStoreIn(prog);
		break;
		case store_reg_to_offset:
			storeRegToOffset(prog);
		break;
		case software_interrupt:
		break;
		case push_pop:
			pushPop(prog);
		break;
		case reti:
		break;
		case sleep:
		break;
		case load_PC_to_reg:
			loadPCtoReg();
		break;
		case jump_indirect: 
		break;
		case djnz:
		break;
		case asmc_im_Reg:
		break;
		case asmc_off_reg:
			asmcOffReg(prog);
		break;
		default:
		break;
	}
	if(instruction!=jump_relative)
		programm_counter++;
}

void Processeur::aluOperation(){
	uint8_t Rs,Rd;
	uint8_t opcode=0;
	opcode = (code_fetched>> 8)&0x0f;
	Rs =  (code_fetched&0x00f0)>>4;
	Rd = code_fetched&0x000f;
	alu.inputS()=registres(Rs);
	std::cout<<"alu operation, Rs:"<<(int)Rs<<":"<<std::hex<<registres(Rs);
	alu.inputD()=registres(Rd);
	std::cout<<" Rd:"<<(int)Rd<<":"<<std::hex<<registres(Rd)<<std::endl;
	if(opcode!=14)
		alu.opcode()=opcode;
	else
		alu.opcode()=11;
	alu.update_state();
	if(opcode!=14)
		registres(Rd)=alu.resultat();
	std::cout<<" resultat: "<<std::hex<<alu.resultat()<<std::endl;
	std::cout<<"cc: "<<std::hex<<(int)alu.condition_reg()<<std::endl;
}

void Processeur::jumpRelative(){
	int16_t off;
	if((code_fetched&0x0400)==0)
		off=code_fetched&(~0x1800);
	else
		off=code_fetched|0xf800;
	programm_counter+=off;
	std::cout<<"relative jump, off: "<<off<<" pc: "<<programm_counter<<std::endl;
}

uint16_t Processeur::codeFetched(){
	return code_fetched;
}

void Processeur::loadRegImm(){
	uint8_t Rn,im;
	Rn=code_fetched&0x0f;
	im=code_fetched>>4;
	std::cout<<"load reg imm Rn: "<<(int)Rn<<":"<<std::hex<<registres(Rn)<<" im: "<<(int)im<<":"<<std::hex<<registres(im)<<std::endl;
	registres(im)=registres(Rn);
	std::cout<<"resultat: Rn: "<<std::hex<<registres(Rn)<<" im: "<<std::hex<<registres(im)<<std::endl;
}

void Processeur::loadStoreIn(Programme& prog){
	uint8_t Rh,Rl,Rn;
	bool ls;
	uint16_t valL,valH;
	uint32_t indirect;
	Rh=(code_fetched&0xf00)>>8;
	Rl=(code_fetched&0xf0)>>4;
	Rn=code_fetched&0x0f;
	ls=code_fetched&0x1000;
	valL=registres(Rl);
	valH=registres(Rh);
	indirect=valL+(valH<<16);
	if(ls){
		registres(Rn)=prog.get(indirect);
		std::cout<<"load indirect to Rn, Rn: "<<(int)Rn<<" Rh: "<<(int)Rh<<":"<<std::hex
		<<valH<<" Rl: "<<(int)Rl<<":"<<std::hex<<valL<<std::endl;
		std::cout<<"indirect: "<<indirect<<":"<<std::hex<<prog.get(indirect)<<std::endl;
		std::cout<<"result: Rn:"<<(int)Rn<<":"<<std::hex<<registres(Rn)<<std::endl;
	}
	else
	{
		prog.put(indirect,registres(Rn));
		std::cout<<"load Rn to indirect, Rn: "<<(int)Rn<<" Rh: "<<(int)Rh<<":"<<std::hex
		<<valH<<" Rl: "<<(int)Rl<<":"<<std::hex<<valL<<std::endl;
		std::cout<<"result: indirect: "<<indirect<<":"<<std::hex<<prog.get(indirect)<<std::endl;
	}
}

void Processeur::storeRegToOffset(Programme& prog){
	int8_t off=(code_fetched>>4);
	uint8_t Rn=code_fetched&0x0f;
	std::cout<<"store reg to offset, Rn:"<<(int)Rn<<":"<<std::hex<<registres(Rn)<<" off: "<<(int)off<<std::endl;
	std::cout<<"PC+off: "<<std::hex<<(programm_counter+off)<<":"<<std::hex<<prog.get(programm_counter+off)<<std::endl;
	prog.put(programm_counter+off,registres(Rn));
	std::cout<<"result: "<<std::hex<<(programm_counter+off)<<":"<<std::hex<<prog.get(programm_counter+off)<<std::endl;
}

void Processeur::pushPop(Programme& prog){
	uint32_t sp=(registres(0x10)<<16)+registres(0x11);
	uint8_t im=(code_fetched&(~0x0080));
	if(code_fetched&0x0080){
		std::cout<<"push, im: "<<(int)im<<":"<<std::hex<<registres(im)<<std::endl;
		prog.put(sp,registres(im));
		sp--;
		std::cout<<"SP: "<<std::hex<<sp<<":"<<std::hex<<prog.get(sp)<<std::endl;
	}
	else{
		registres(im)=prog.get(sp);
		sp++;
		std::cout<<"pop, SP: "<<std::hex<<sp<<":"<<std::hex<<prog.get(sp)<<std::endl;
		std::cout<<"im: "<<(int)im<<":"<<std::hex<<registres(im)<<std::endl;
	}
	registres(0x10)=(sp>>16);
	registres(0x11)=(sp);
}

void Processeur::asmcOffReg(Programme prog){
	uint8_t op=(code_fetched>>12)&0x03;
	int8_t off=code_fetched>>4;
	uint8_t Rn=code_fetched&0x0f;
	uint8_t aluOp;
	std::cout<<"add sub mov comp off reg\n"<<
	"Rn: "<<(int)Rn<<":"<<std::hex<<registres(Rn)<<"\n"<<
	"off: "<<(int)off<<" PC+off: "<<std::hex<<programm_counter+off<<":"<<
	std::hex<<prog.get(programm_counter+off)<<std::endl;
	std::cout<<"operation: "<<(int)op<<std::endl;
	if (op==1||op==3)
	{
		aluOp=11;
	}
	else
		aluOp=10;
	if(op!=2){
		alu.inputS()=registres(Rn);
		alu.inputD()=prog.get(programm_counter+off);
		alu.opcode()=aluOp;
		alu.update_state();
		if (op!=3)
		{
			registres(Rn)=alu.resultat();
		}
	}
	else{
		registres(Rn)=prog.get(programm_counter+off);
	}
	std::cout<<"result: Rn:"<<(int)Rn<<":"<<std::hex<<registres(Rn)<<"\n"<<
	"condition reg: "<<std::hex<<(unsigned int)alu.condition_reg()<<std::endl;
}

void Processeur::loadPCtoReg(){
	uint8_t Rh,Rl;
	Rh=(code_fetched>>4)&0x0f;
	Rl=code_fetched&0x0f;
	registres(Rh)=programm_counter>>16;
	registres(Rl)=programm_counter;
	std::cout<<"load PC to reg, Rh: "<<(int)Rh<<":"<<std::hex<<registres(Rh)<<
	" Rl: "<<(int)Rl<<":"<<std::hex<<registres(Rl)<<std::endl;
}