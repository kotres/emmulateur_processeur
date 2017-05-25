#pragma once
#ifndef ALU_HPP
#define ALU_HPP
#include <cstdint>

class ALU{
private:
	uint16_t minputS,minputD,mresultat;
	uint8_t mcondition_reg;
	uint8_t mopcode;
public:
	void update_state();
	uint16_t& inputS();
	uint16_t inputS() const;
	uint16_t& inputD();
	uint16_t inputD() const;
	uint8_t& opcode();
	uint8_t opcode() const;
	uint16_t resultat() const;
	uint8_t condition_reg() const;
};

#endif
