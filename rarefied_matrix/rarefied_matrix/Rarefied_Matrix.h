#pragma once

#include <iostream>

struct Item {
	int str;
	int col;
	double value;

	Item(int str, int col, double value);
};

class Rarefied_Matrix {
public:
	struct Elem {
		Elem* next;
		Elem* prev;
		Item data;

		Elem(int str, int col, double value);
		bool operator<(Elem& right);
		bool operator==(Elem& right);
	};

	Elem* first;
	Elem* last;

	Rarefied_Matrix();
	Rarefied_Matrix(const Rarefied_Matrix &m);
	~Rarefied_Matrix();

	void add_elem(int str, int col, double value);
	void del_elem(Elem* el);
	void sort();
	void print();

	Rarefied_Matrix operator+(Rarefied_Matrix& m2);
private:
	
	
};