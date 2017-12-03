#include "Deq.h"

Deq::Deq() : begin(nullptr), tail(nullptr) {
	std::cout << "In Deq()" << std::endl;
}

Deq::Deq(Fraction& b, Fraction& t) : Deq() {
	push_back(b);
	push_back(t);
	std::cout << "In Deq(Fraction&, Fraction&)" << std::endl;
}

Deq::~Deq() {
	if (begin != nullptr) {
		Fraction* it = begin;
		while (it->next != nullptr) {
			Fraction* temp = it->next;
			delete it;
			it = temp;
		}
		delete it;
	}
	std::cout << "In ~Deq()" << std::endl;
}

void Deq::push_forward(Fraction& f) {
	Fraction* temp = nullptr;
	/*if (typeid(Fraction) == typeid(f)) {
		temp = new Fraction(f);
	}
	if (typeid(FloorFraction) == typeid(f)) {
		temp = new FloorFraction(f);
	}
	if (typeid(CeilFraction) == typeid(f)) {
		temp = new CeilFraction(f);
	}*/
	temp = (&f)->copy();
	temp->next = begin;
	begin = temp;
	if (tail == nullptr) {
		tail = begin;
	}
}

void Deq::push_back(Fraction& f) {
	Fraction* temp = nullptr;
	/*if (typeid(Fraction) == typeid(f)) {
		temp = new Fraction(f);
	}
	if (typeid(FloorFraction) == typeid(f)) {
		temp = new FloorFraction(f);
	}
	if (typeid(CeilFraction) == typeid(f)) {
		temp = new CeilFraction(f);
	}*/
	temp = (&f)->copy();
	if (tail != nullptr) {
		tail->next = temp;
	}
	tail = temp;
	if (begin == nullptr) {
		begin = tail;
	}
}

void Deq::pop_forward() {
	if (begin == nullptr) {
		std::cout << "Deq is empty" << std::endl;
		//Лабораторная работа №6.
		throw std::runtime_error("Deq is empty");
	}
	Fraction* f = begin;
	begin = begin->next;
	delete f;
}

Fraction* Deq::search(int n, unsigned int d) {
	if (begin == nullptr) {
		std::cout << "Deq is empty" << std::endl;
		//Лабораторная работа №6.
		throw std::runtime_error("Deq is empty");
	}
	Fraction* it = begin;
	while (it != tail) {
		if (it->numerator == n && it->denominator == d)
		{
			it->print();
			return it;
		}
		it = it->next;
		if (it == begin) {
			std::cout << "No such Fraction" << std::endl;
			return nullptr;
		}
	}

	if (it->numerator == n && it->denominator == d)
		return it;

	std::cout << "No such Fraction" << std::endl;
	return nullptr;
}

void Deq::print_all() {
	if (begin == nullptr) {
		std::cout << "Deq is empty" << std::endl;
		//Лабораторная работа №6.
		throw std::runtime_error("Deq is empty");
	}
	Fraction* it = begin;
	while (it != tail) {
		it->print();
		it = it->next;
		if (it == begin)
			return;
	}
	it->print();
}
