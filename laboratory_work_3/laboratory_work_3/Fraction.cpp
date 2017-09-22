﻿#include "Fraction.h"

Fraction::Fraction() : numerator(0), denominator(1), name(nullptr) {
	std::cout << "In Fraction()" << std::endl;
}

Fraction::Fraction(int nu, unsigned int d, const char *n) : numerator(nu), denominator(d) {
	unsigned int ln = strlen(n);
	name = new char[ln + 1];
	memcpy(name, n, ln + 1);

	std::cout << "In Fraction(int, int, const char*)" << std::endl;
}

Fraction::Fraction(const Fraction &f) : numerator(f.numerator), denominator(f.denominator) {
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

void Fraction::concat(char *&a, const char *b) {
	if (b == nullptr) {
		return;
	}

	unsigned int ln2 = strlen(b);
	if (ln2 == 0) {
		return;
	}
	unsigned int ln1 = 0;
	if (a != nullptr) {
		ln1 = strlen(a);
	}
	char *ch = new char[ln1 + ln2 + 1];
	if (a != nullptr) {
		memcpy(ch, a, ln1);
	}
	memcpy(ch + ln1, b, ln2 + 1);
	delete[] a;
	a = ch;
}

Fraction Fraction::addition(const Fraction &a) {
	if (name == nullptr || a.name == nullptr) {
		std::cout << "Wrong numbers" << std::endl;
		return Fraction();
	}
	int nu = numerator*a.denominator + a.numerator * denominator;
	unsigned int d = denominator * a.denominator;

	char *ch = nullptr;
	concat(ch, name);
	concat(ch, " + ");
	concat(ch, a.name);

	Fraction f(nu, d, ch);
	return f;
}

const char* Fraction::get_name() const {
	if (name == nullptr) {
		std::cout << "Wrong number" << std::endl;
		return nullptr;
	}
	/*const int max_num = 22;
	unsigned int ln_n = strlen(name);
	char *ch = new char[max_num + ln_n + 4];
	_itoa(numerator, ch, 10);
	unsigned int ln = strlen(ch);

	if (denominator != 1 && numerator != 0) {
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

	return returning;*/
	return name;
}

void Fraction::print() {
	const char *n = get_name();
	if (n != nullptr) {

		constexpr int max_num = 22;
		char ch[max_num + 1];
		_itoa(numerator, ch, 10);
		unsigned int ln = strlen(ch);

		if (denominator != 1 && numerator != 0) {
			ch[ln] = '/';
			_itoa(denominator, ch + ln + 1, 10);
			ln = strlen(ch);
		}

		std::cout << "value = " << ch << " (" << n << ")" << std::endl;
		/*delete[] ch;*/
	}
}

Fraction Fraction::operator=(const Fraction &a) {
	if (this == &a) {
		return *this;
	}
	if (name != nullptr) {
		delete[] name;
	}
	numerator = a.numerator;
	denominator = a.denominator;
	unsigned int ln = strlen(a.name);
	name = new char[ln + 1];
	memcpy(name, a.name, ln + 1);
	return *this;
}

Fraction Fraction::operator+(const Fraction &a) {
	return addition(a);
}

Fraction operator-(const Fraction &a, const Fraction &b) {
	if (a.name == nullptr || b.name == nullptr) {
		std::cout << "Wrong numbers" << std::endl;
		return Fraction();
	}
	int nu = a.numerator*b.denominator - b.numerator * a.denominator;
	unsigned int d = a.denominator * b.denominator;

	char *ch = nullptr;
	Fraction::concat(ch, a.name);
	Fraction::concat(ch, " - ");
	Fraction::concat(ch, b.name);

	Fraction f(nu, d, ch);
	return f;
}

Fraction& Fraction::operator++() {
	numerator += denominator;
	concat(name, " + ");
	concat(name, "один");
	return *this;
}

Fraction Fraction::operator++(int) {
	Fraction temp(*this);
	numerator += denominator;
	concat(name, " + ");
	concat(name, "один");
	return temp;
}

Fraction::operator double() const {
	return (double)numerator / denominator;
}

std::ostream& operator<<(std::ostream &os, Fraction &f) {
	const char *n = f.get_name();
	if (n != nullptr) {

		constexpr int max_num = 22;
		char ch[max_num + 1];
		_itoa(f.numerator, ch, 10);
		unsigned int ln = strlen(ch);

		if (f.denominator != 1 && f.numerator != 0) {
			ch[ln] = '/';
			_itoa(f.denominator, ch + ln + 1, 10);
			ln = strlen(ch);
		}

		os << ch << " (" << n << ")";
	}
	return os;
}

std::istream& operator>>(std::istream &is, Fraction &f) {
	is >> f.numerator;
	char ch = is.get();
	if (ch == '/') {
		is >> f.denominator;
	}

	int i = -1;
	std::streampos s;
	while ((ch = is.get()) != ')') {
		if (ch == '(') {
			i = 0;
			s = is.tellg();
			continue;
		}

		if (i >= 0) {
			i++;
		}
	}

	is.seekg(s);
	if (f.name != nullptr) {
		delete[] f.name;
	}
	f.name = new char[i + 1];
	f.name[i] = '\0';
	int j = 0;
	while (i-- > 0) {
		f.name[j++] = is.get();
	}
	is.get();
	return is;
}

void Fraction::write(std::ostream &os) {
	os.write(reinterpret_cast<const char*>(&numerator), sizeof(numerator));
	os.write(reinterpret_cast<const char*>(&denominator), sizeof(denominator));
	os.write(name, strlen(name) + 1);
}

void Fraction::read(std::istream &is) {
	is.read(reinterpret_cast<char*>(&numerator), sizeof(numerator));
	is.read(reinterpret_cast<char*>(&denominator), sizeof(denominator));

	char ch;
	int i = 0;
	std::streampos s = is.tellg();;
	while ((ch = is.get()) != '\0') {
			i++;
	}

	is.seekg(s);
	if (name != nullptr) {
		delete[] name;
	}
	name = new char[i + 1];
	is.read(name, i + 1);
}