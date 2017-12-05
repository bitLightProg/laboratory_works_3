#pragma once

#include <iostream>
#include <ostream>
#include <istream>
#include <cmath>
#include <exception>

#include "InterBase.h"

class Fraction: public InterBase {
public:
	int size_of_data() override;
	void* get_data() override;

	// Лабораторная работа №1.
	Fraction();
	Fraction(int nu, unsigned int d, const char* n);
	Fraction(const Fraction& f);
	~Fraction();

	const char* get_name() const;
	//...1

	//Лабораторная работа №2.
	Fraction operator=(const Fraction& a);
	Fraction operator+(const Fraction& a);
	friend Fraction operator-(const Fraction& a, const Fraction& b);
	Fraction& operator++();
	Fraction operator++(int);

	bool operator<(const Fraction& a);

	operator double() const;
	//...2
	//Лабораторная работа №3.
	friend std::ostream& operator<<(std::ostream& os, const Fraction& f);
	friend std::istream& operator>>(std::istream& is, Fraction& f);
	void read(std::istream& is);
	void write(std::ostream& os);
	//...3
	//Лабораторная работа №1, №4
	virtual void print();
	//Лабораторная работа №5
	virtual Fraction* copy();
	bool operator==(Fraction& f);

protected:
	//Лабораторная работа №1.
	inline Fraction addition(const Fraction& a);
	//...1
	//Вспомогательная функция.
	static inline void concat(char*& a, const char* b);

	int numerator;
	unsigned int denominator;
	char *name;
};