#pragma once
#include "Fraction.h"
class FloorFraction :
	public Fraction {
public:
	FloorFraction();
	FloorFraction(int nu, unsigned int d, const char *n);
	FloorFraction(const FloorFraction &f);
	~FloorFraction();

	FloorFraction(const Fraction &f);

	void print() const;

private:
	int floor_numb;
};

