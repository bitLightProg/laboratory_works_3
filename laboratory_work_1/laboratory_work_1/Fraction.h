#pragma once
#include <iostream>

class Fraction {
public:
	Fraction();
	Fraction(int nu, unsigned int d, const char *n);
	Fraction(const Fraction &f);
	~Fraction();

	Fraction* addition(const Fraction &a);
	char* c_str();
	void print();

private:
	int numerator;
	unsigned int denominator;
	char *name;
};