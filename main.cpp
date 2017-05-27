#include <iostream>
#include "programme.hpp"
#include "processeur.hpp"

int main(){
	char quitter;
	Processeur proc;
	Programme prog("mon_code.hex");
	while(quitter!='q'){
		proc.fetch(prog);
		proc.decode();
		proc.execute();
		std::cin>>quitter;
		std::cout<<std::endl;
	}
	return 0; 
}
