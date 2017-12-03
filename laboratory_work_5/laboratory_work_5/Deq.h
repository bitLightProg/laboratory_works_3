#pragma once

#include <typeinfo>
#include <iostream>
#include "Fraction.h"
#include "FloorFraction.h"
#include "CeilFraction.h"

class Deq {
public:
	Deq();
	Deq(Fraction& b, Fraction& t);
	~Deq();

	Deq(const Deq&) = delete;

	void push_forward(Fraction& f);
	void push_back(Fraction& f);
	void pop_forward();
	Fraction* search(int n, unsigned int d);
	void print_all();

private:
	Fraction* begin;
	Fraction* tail;
};

