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
	// alu.opcode()=RR;
	// for(uint16_t i =0x0;i<28;i++)
	// {
	// 	alu.inputR()=i;
	// 	alu.update_state();
	// 	std::cout<<std::hex<<alu.resultat()<<std::endl;
	// }
	return 0; 
}
