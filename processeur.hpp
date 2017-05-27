#pragma once
#ifndef PROCESSEUR_HPP
#define PROCESSEUR_HPP

#include <list>
#include <cstdint>
#include <array>

#include "ALU.hpp"
#include "programme.hpp"
#include "instruction.hpp"

class Processeur
{
private:
	ALU alu;
	std::array<uint16_t,16> registres;
	uint32_t programm_counter;
	uint32_t stack_pointer;
	std::array<uint16_t,3> code_fetched;
public:
	void fetch(Programme prog);
	void decode();
	void execute();
	Processeur();
};

#endif