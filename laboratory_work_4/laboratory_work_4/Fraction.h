#pragma once
#include <iostream>
#include <ostream>
#include <istream>

class Fraction {
public:
	Fraction();
	Fraction(int nu, unsigned int d, const char *n);
	Fraction(const Fraction &f);
	~Fraction();

	const char* get_name() const;

	Fraction operator=(const Fraction &a);
	Fraction operator+(const Fraction &a);
	friend Fraction operator-(const Fraction &a, const Fraction &b);
	Fraction& operator++();
	Fraction operator++(int);

	operator double() const;

	friend std::ostream& operator<<(std::ostream &os, Fraction &f);
	friend std::istream& operator>>(std::istream &is, Fraction &f);
	void read(std::istream &is);
	void write(std::ostream &os);

	virtual void print() const;

protected:
	inline Fraction addition(const Fraction &a);
	static inline void concat(char *&a, const char *b);

	int numerator;
	unsigned int denominator;
	char *name;
};