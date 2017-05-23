#pragma once
#ifndef PROCESSEUR_HPP
#define PROCESSEUR_HPP

#include "registres.hpp"
#include "ALU.hpp"
#include "programme.hpp"

class Processeur
{
private:
	ALU alu;
	Registres registres;
	unsigned int programm_counter;
	unsigned short int code_fetched;
	unsigned char Ra,Rb;
	void alu_operation();
public:
	void fetch(Programme prog);
	void decode();
	void execute();
	Processeur();
	unsigned short int codeFetched();
};

#endif