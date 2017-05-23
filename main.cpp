#include <iostream>
#include "programme.hpp"
#include "processeur.hpp"

int main(){
	Processeur proc;
	Programme prog("mon_code.hex");
	for(unsigned int i=0;i<prog.taille();i++){
		proc.fetch(prog);
		proc.decode();
		proc.execute();
	}
	return 0; 
}
