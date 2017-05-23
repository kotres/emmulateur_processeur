#include "registres.hpp"

unsigned short int& Registres::registre(unsigned int i){
	return data.at(i);
}

unsigned short int Registres::registre(unsigned int i) const{
	return data.at(i);
}