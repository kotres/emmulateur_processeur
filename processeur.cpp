#include "processeur.hpp"
#include <iostream>

#define RETI 0x8200

Processeur::Processeur()
:alu(),registres(),programm_counter(0),code_fetched(0),Ra(0),Rb(0)
{}

void Processeur::fetch(Programme prog){
	code_fetched=prog.get(programm_counter);
	programm_counter++;
}

void Processeur::decode(){
	if((code_fetched>>14)<2){
		if((code_fetched>>11)<4){
			if((code_fetched>>12)==0){
				if(code_fetched!=0){
					//alu_operation();
					std::cout<<"alu operation"<<std::endl;
				}
				else
				std::cout<<"nop"<<std::endl;
			}
			if((code_fetched>>11)==2){
				//conditional jump
				std::cout<<"conditional jump"<<std::endl;
			}
			if((code_fetched>>11)==3){
				//relative jump
				//programm_counter+=(short int)(code_fetched&0x07ff);
				std::cout<<"relative jump"<<std::endl;
			}

		}
		else{
			if((code_fetched>>13)==1){
				//load reg to imm
				std::cout<<"load reg to im"<<std::endl;
			}
			if((code_fetched>>13)==2){
				//load/store indirect
				std::cout<<"load store indirect"<<std::endl;
			}
			if((code_fetched>>13)==3){
				//load/store with offset
				std::cout<<"load/store with offset"<<std::endl;
			}
		}

	}
	else{
		if(code_fetched<=RETI+3){
			if(code_fetched<RETI){
				if((code_fetched>>8)==0x80){
					//software interrupt
					std::cout<<"software interrupt"<<std::endl;
				}
				else{
					//push pop
					std::cout<<"push pop"<<std::endl;
				}
			}
			else{
				if(code_fetched==RETI){
					std::cout<<"return from interrupt"<<std::endl;
				}
				else{
					std::cout<<"push pop PC"<<std::endl;
				}
			}
		}
		else{
			if((code_fetched>>12)<9){
				if((code_fetched>>8)==0x83){
					std::cout<<"load PC to reg"<<std::endl;
				}
				else{
					std::cout<<"jump indirect"<<std::endl;
				}
			}
			else{
				if((code_fetched>>12)==9){
					std::cout<<"decrease jump not zero"<<std::endl;
				}
				if((code_fetched>>13)==5){
					std::cout<<"add/sub/mov/comp imto reg"<<std::endl;
				}
				if((code_fetched>>14)==3){
					std::cout<<"add/sub/mov/comp byte to reg"<<std::endl;
				}
			}
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
	//std::cout<<(unsigned int)opcode<<" "<<(unsigned int)Ra<<" "<<(unsigned int)Rb<<std::endl;
}

unsigned short int Processeur::codeFetched(){
	return code_fetched;
}