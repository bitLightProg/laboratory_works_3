#pragma once

#include "Fraction.h"

class CeilFraction :
	public Fraction {
public:
	CeilFraction();
	CeilFraction(int nu, unsigned int d, const char* n, Fraction* next = nullptr);
	CeilFraction(const CeilFraction& f);
	~CeilFraction();

	CeilFraction(const Fraction& f);

	void print();
	Fraction* copy();

private:
	int ceil_numb;
	//Лабораторная работа №5.
	Fraction* next;
};