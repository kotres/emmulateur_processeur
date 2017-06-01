#include <iostream>
#include "programme.hpp"
#include "processeur.hpp"

int main(){
	char quitter;
	Processeur proc;
	Programme prog("mon_code.hex");
	while(quitter!='q'){
		proc.clock_cycle(prog);
		std::cin>>quitter;
		std::cout<<std::endl;
	}
	// ALU alu;
	// alu.inputL()=0x01;
	// alu.opcode()=RRC;
	// for(uint16_t i =0x0;i<28;i++)
	// {
	// 	alu.inputR()=0x01;
	// 	alu.update_state();
	// 	alu.inputL()=alu.resultat();
	// 	std::cout<<"i="<<i<<std::endl;
	// 	std::cout<<std::hex<<alu.resultat()<<std::endl;
	// 	alu.condition_reg()&=0x01;
	// 	std::cout<<std::hex<<(int)alu.condition_reg()<<std::endl;
	// }
	return 0; 
}
