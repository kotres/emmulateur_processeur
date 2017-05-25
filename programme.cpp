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
		unsigned short int data;
		unsigned int address=0;
		while(!fichier.eof()){
			fichier>>std::hex>>data;
			code[address]=data;
			address++;
		}
	}
}

unsigned short int Programme::get(unsigned int i){
	if(code.find(i)==code.end()){
		std::random_device rd;
		std::mt19937 mt(rd());
    	std::uniform_int_distribution<unsigned int> dist(0, 0xffff);
    	code[i]=dist(mt);
	}
	return code.at(i);
}

void Programme::put(unsigned int i,unsigned short int data){
	code[i]=data;
}

unsigned int Programme::taille(){
	return code.size();
}