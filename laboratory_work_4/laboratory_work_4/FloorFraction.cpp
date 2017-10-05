#include "FloorFraction.h"



FloorFraction::FloorFraction() : Fraction(), floor_numb(0) {
	std::cout << "In FloorFraction()" << std::endl;
}

FloorFraction::FloorFraction(int nu, unsigned int d, const char *n)
	: Fraction(nu, d, n), floor_numb(floor((float)nu / d)) {
	std::cout << "In FloorFraction(int, uint, const char*)" << std::endl;
}

FloorFraction::FloorFraction(const Fraction &f)
	: Fraction(f), floor_numb(floor((float)numerator / denominator)) {
	std::cout << "In FloorFraction(const Fraction &f)" << std::endl;
}

FloorFraction::FloorFraction(const FloorFraction & f)
	: FloorFraction(static_cast<Fraction>(f)) {
	std::cout << "In FloorFraction(const FloorFraction &f)" << std::endl;
}

FloorFraction::~FloorFraction() {
	std::cout << "In ~FloorFraction()" << std::endl;
}

void FloorFraction::print() const {
	std::cout << "Floor = " << floor_numb << " ";
	Fraction::print();
}