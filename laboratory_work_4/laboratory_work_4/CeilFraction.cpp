#include "CeilFraction.h"



CeilFraction::CeilFraction(): Fraction(), ceil_numb(0) {
	std::cout << "In CeilFraction()" << std::endl;
}

CeilFraction::CeilFraction(int nu, unsigned int d, const char *n)
	: Fraction(nu,d,n), ceil_numb(ceil((float)nu / d)) {
	std::cout << "In CeilFraction(int, uint, const char*)" << std::endl;
}

CeilFraction::CeilFraction(const Fraction &f)
	: Fraction(f), ceil_numb(ceil((float)numerator / denominator)) {
	std::cout << "In CeilFraction(const Fraction &f)" << std::endl;
}

CeilFraction::CeilFraction(const CeilFraction & f)
	: CeilFraction(static_cast<Fraction>(f)) {
	std::cout << "In CeilFraction(const CeilFraction &f)" << std::endl;
}

CeilFraction::~CeilFraction() {
	std::cout << "In ~CeilFraction()" << std::endl;
}

void CeilFraction::print() const {
	std::cout << "Ceil = " << ceil_numb << " ";
	Fraction::print();
}