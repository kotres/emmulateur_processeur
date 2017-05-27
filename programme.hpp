#pragma once
#ifndef PROGRAMME_HPP
#define PROGRAMME_HPP

#include <map>
#include <string>
#include <cstdint>

class Programme
{
	std::map<uint32_t,uint16_t> code;
public:
	uint16_t& operator()(uint32_t address);
	unsigned int taille();
	Programme();
	Programme(std::string filename);
};

#endif