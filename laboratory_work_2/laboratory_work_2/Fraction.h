#pragma once
#include <iostream>

class Fraction {
public:
	Fraction();
	Fraction(int nu, unsigned int d, const char *n);
	Fraction(const Fraction &f);
	~Fraction();

	
	const char* get_name() const;
	void print();

	Fraction operator=(const Fraction &a);
	Fraction operator+(const Fraction &a);
	friend Fraction operator-(const Fraction &a, const Fraction &b);
	Fraction operator++();
	Fraction operator++(int);
	operator float() const;
	operator double() const;
	
private:
	inline Fraction addition(const Fraction &a);
	static inline void concat(char *&a, const char *b);

	int numerator;
	unsigned int denominator;
	char *name;
};