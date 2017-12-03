#pragma once

#include "Fraction.h"

class FloorFraction :
	public Fraction {
public:
	FloorFraction();
	FloorFraction(int nu, unsigned int d, const char* n, Fraction* next = nullptr);
	FloorFraction(const FloorFraction& f);
	~FloorFraction();

	FloorFraction(const Fraction& f);

	void print();
	Fraction* copy();

private:
	int floor_numb;
	//Лабораторная работа №5.
	Fraction* next;
};