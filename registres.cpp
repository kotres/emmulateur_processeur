#include "registres.hpp"

unsigned short int& Registres::operator()(unsigned int i){
	return data.at(i);
}