#include "programme.hpp"

#include <fstream>

Programme::Programme()
:code(1,0)
{}

Programme::Programme(std::string filename)
:code(1,0)
{
	std::ifstream fichier(filename);
	if(fichier.is_open()){
		code.clear();
		unsigned short int data;
		while(!fichier.eof()){
			fichier>>std::hex>>data;
			code.push_back(data);
		}
	}
}

unsigned short int Programme::get(unsigned int i){
	if(i>=code.size())
		return 0;
	return code.at(i);
}

void Programme::put(unsigned int i,unsigned short int data){
	if(i>=code.size()&&i<0x1ffff)
		code.resize(i+1,0);
	code.at(i)=data;
}

unsigned int Programme::taille(){
	return code.size();
}