#include <iostream>
#include "programme.hpp"
#include "processeur.hpp"

int main(){
	//char quitter;
	//Processeur proc;
	//Programme prog("mon_code.hex");
	// while(quitter!='q'){
	// 	proc.clock_cycle(prog);
	// 	std::cin>>quitter;
	// 	std::cout<<std::endl;
	// }
	ALU alu;
	alu.inputL()=0xff13;
	alu.inputR()=0x5555;
	char q=0;
	unsigned int i=0;
	while(q!='q'){
		std::cout<<"left"<<std::endl;
		std::cin>>std::hex>>alu.inputL();
		std::cout<<"right"<<std::endl;
		std::cin>>std::hex>>alu.inputR();
		std::cout<<"opcode"<<std::endl;
		std::cin>>i;
		alu.opcode()=(ALU_opcode)i;
		alu.update_state();
		std::cout<<alu.resultat()<<std::endl;
		std::cout<<(int)alu.condition_reg()<<std::endl;
		alu.condition_reg()=0;
		std::cout<<"quit"<<std::endl;
		std::cin>>q;
	}
	return 0; 
}
