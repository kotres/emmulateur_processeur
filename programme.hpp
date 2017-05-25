#pragma once
#ifndef PROGRAMME_HPP
#define PROGRAMME_HPP

#include <map>
#include <string>

class Programme
{
	std::map<unsigned int,unsigned short int> code;
public:
	unsigned short int get(unsigned int i);
	void put(unsigned int i,unsigned short int data);
	unsigned int taille();
	Programme();
	Programme(std::string filename);
};

#endif