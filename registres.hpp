#pragma once
#ifndef REGISTRES_HPP
#define REGISTRES_HPP
#include <array>

class Registres
{
private:
	std::array<unsigned short int,128> data;
public:
	unsigned short int& registre(unsigned int i);
	unsigned short int registre(unsigned int i) const;
	unsigned short int& operator()(unsigned int i);
};

#endif