#include "programme.hpp"

#include <fstream>
#include <random>

Programme::Programme()
:code()
{}

Programme::Programme(std::string filename)
:code()
{
	std::ifstream fichier(filename);
	if(fichier.is_open()){
		code.clear();
		uint16_t data;
		uint32_t address=0;
		while(!fichier.eof()){
			fichier>>std::hex>>data;
			code[address]=data;
			address++;
		}
	}
}

uint16_t Programme::get(uint32_t address){
	if(code.find(address)==code.end()){
		std::random_device rd;
		std::mt19937 mt(rd());
    	std::uniform_int_distribution<unsigned int> dist(0, 0xffff);
    	code[address]=dist(mt);
	}
	return code.at(address);
}

void Programme::put(uint32_t address,uint16_t data){
	code[address]=data;
}

unsigned int Programme::taille(){
	return code.size();
}