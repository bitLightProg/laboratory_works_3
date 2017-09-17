#include "Fraction.h"

Fraction::Fraction(): numerator(0), denominator(1), name(nullptr) {
	std::cout << "In Fraction()" << std::endl;
}

Fraction::Fraction(int nu, unsigned int d, const char *n): numerator(nu), denominator(d) {
	unsigned int ln = strlen(n);
	name = new char[ln + 1];
	memcpy(name, n, ln + 1);	

	std::cout << "In Fraction(int, int, const char*)" << std::endl;
}

Fraction::Fraction(const Fraction &f): numerator(f.numerator), denominator(f.denominator) {
	unsigned int ln = strlen(f.name);
	name = new char[ln + 1];
	memcpy(name, f.name, ln + 1);	

	std::cout << "In Fraction(const Fraction&)" << std::endl;
}

Fraction::~Fraction() {
	if (name != nullptr) {
		delete[] name;
	}	
	std::cout << "In ~Fraction()" << std::endl;
}

Fraction* Fraction::addition(const Fraction &a) {
	if (name == nullptr || a.name == nullptr) {
		std::cout << "Wrong numbers" << std::endl;
		return nullptr;
	}
	int nu = numerator*a.denominator + a.numerator * denominator;
	unsigned int d = denominator * a.denominator;

	unsigned int ln1 = strlen(name);
	unsigned int ln = ln1 + strlen(a.name) + 4;
	char *n = new char[ln + 1];

	memcpy(n, name, ln1);
	n[ln1] = ' ';
	n[ln1 + 1] = '+';
	n[ln1 + 2] = ' ';

	memcpy(n + ln1 + 3, a.name, ln - ln1 + 1);

	Fraction *f = new Fraction(nu, d, n);
	return f;
}

char* Fraction::c_str() {
	if (name == nullptr) {
		std::cout << "Wrong number" << std::endl;
		return nullptr;
	}
	const int max_num = 22;
	unsigned int ln_n = strlen(name);
	char *ch = new char[max_num + ln_n + 4];
	_itoa(numerator, ch, 10);
	unsigned int ln = strlen(ch);

	if (denominator != 1) {
		ch[ln] = '/';
		_itoa(denominator, ch + ln + 1, 10);
		ln = strlen(ch);
	}	

	ch[ln] = ' ';
	ch[ln + 1] = '(';
	memcpy(ch + ln + 2, name, ln_n + 1);
	ln = strlen(ch);
	ch[ln] = ')';
	ch[ln + 1] = '\0';

	char *returning = new char[ln + 3];
	memcpy(returning, ch, ln + 3);

	delete[] ch;

	return returning;
}

void Fraction::print() {
	char *ch = c_str();
	if (ch != nullptr) {
		std::cout << "value = " << ch << std::endl;
		delete[] ch;
	}

}