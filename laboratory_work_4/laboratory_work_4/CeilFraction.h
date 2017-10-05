#pragma once
#include <cmath>

#include "Fraction.h"

class CeilFraction :
	public Fraction {
public:
	CeilFraction();
	CeilFraction(int nu, unsigned int d, const char *n);
	CeilFraction(const CeilFraction &f);
	~CeilFraction();

	CeilFraction(const Fraction &f);

	void print() const;

private:
	int ceil_numb;
};

