#include "Rarefied_Matrix.h"

Item::Item(int s, int c, double v): str(s), col(c), value(v) {
}

Rarefied_Matrix::Elem::Elem(int s, int c, double v) : next(nullptr), prev(nullptr), data(s, c, v) {
}

bool Rarefied_Matrix::Elem::operator<(Elem& right) {
	if (data.str < right.data.str)
		return true;
	else if (data.col < right.data.col)
		return true;
	else return false;
}

bool Rarefied_Matrix::Elem::operator==(Elem& right) {
	if (data.str == right.data.str && data.col == right.data.col)
		return true;
	else return false;
}

Rarefied_Matrix::Rarefied_Matrix(): first(nullptr), last(nullptr) {
}

Rarefied_Matrix::Rarefied_Matrix(const Rarefied_Matrix &m) {
	Elem* it = m.first;
	while (it != nullptr) {
		this->add_elem(it->data.str, it->data.col, it->data.value);
		it = it->next;
	}
}

Rarefied_Matrix::~Rarefied_Matrix() {
	Elem* it = first;
	while (it != nullptr) {
		Elem* temp = it->next;
		delete it;
		it = temp;
	}
	first = last = nullptr;
}

void Rarefied_Matrix::add_elem(int str, int col, double value) {
	if (value == 0)
		return;
	if (first == nullptr) {
		first = last = new Elem(str, col, value);
		return;
	}
	Elem* it = first;
	Elem el(str, col, 1);
	while (*it < el && it->next != nullptr)
		it = it->next;
	/*while (it->data.str < str && it->next != nullptr)
		it = it->next;
	while (it->data.col < col && it->next != nullptr)
		it = it->next;*/
	if (it->next == nullptr) {
		Elem el(str, col, 1);
		if (*it == el)
			it->data.value += value;
		else if (*it < el) {
			it->next = new Elem(str, col, value);
			it->next->prev = it;
		}
		else {
			Elem* e = new Elem(str, col, value);
			e->next = it;
			e->prev = it->prev;
			if (e->prev != nullptr)
				e->prev->next = e;
			it->prev = e;
		}
			

	}
	else if (it->data.col < col) {
		Elem* temp = it->next;
		it->next = new Elem(str, col, value);
		it->next->prev = it;
		it->next->next = temp;
	}
	else if (it->data.col > col) {
		Elem* temp = it->prev;
		it->prev = new Elem(str, col, value);
		it->prev->next = it;
		it->prev->prev = temp;
	}
	else {
		it->data.value += value;
	}
}

void Rarefied_Matrix::del_elem(Rarefied_Matrix::Elem* el) {
	//Elem* temp = el->prev;
	if (el == first)
		first = first->next;
	if (el == last)
		last = last->prev;
	if (el->prev != nullptr && el->next != nullptr) {
		el->prev->next = el->next;
		el->next->prev = el->prev;
	}
	else if (el->prev == nullptr && el->next == nullptr) {
		first = last = nullptr;
	}
	else if (el->prev == nullptr) {
		el->next->prev = nullptr;
	}
	else {
		el->prev->next = nullptr;
	}
	delete el;
}

void Rarefied_Matrix::sort() {
	//TODO:Ускорить.
	Rarefied_Matrix temp;
	Elem* it = first;
	while (it != nullptr) {
		temp.add_elem(it->data.str, it->data.col, it->data.value);
		Elem* temp_el = it->next;
		del_elem(it);
		it = temp_el;
	}
	it = temp.first;
	while (it != nullptr) {
		add_elem(it->data.str, it->data.col, it->data.value);
		it = it->next;
	}
}

void Rarefied_Matrix::print() {
	Elem* it = first;
	while (it != nullptr) {
		std::cout << "["<<it->data.str << "," << it->data.col << "]: " << it->data.value << std::endl;
		it = it->next;
	}
}

Rarefied_Matrix Rarefied_Matrix::operator+(Rarefied_Matrix& m2) {
	sort();
	m2.sort();
	Rarefied_Matrix ret;
	Elem* t1 = first;
	Elem* t2 = m2.first;
	cycle:
	while ((t1 != nullptr && t2 != nullptr) && (/*!(t1 == t2))*/(t1->data.str != t2->data.str) || (t1->data.col != t2->data.col))) {
		/*if (t1 < t2) {
			ret.add_elem(t1->data.str, t1->data.col, t1->data.value);
			t1 = t1->next;
		}
		else {
			ret.add_elem(t2->data.str, t2->data.col, t2->data.value);
			t2 = t2->next;
		}*/
		if (t1->data.str < t2->data.str) {
			ret.add_elem(t1->data.str, t1->data.col, t1->data.value);
			t1 = t1->next;
		}
		else if (t1->data.str > t2->data.str) {
			ret.add_elem(t2->data.str, t2->data.col, t2->data.value);
			t2 = t2->next;
		}
		else {
			if (t1->data.col < t2->data.col) {
				ret.add_elem(t1->data.str, t1->data.col, t1->data.value);
				t1 = t1->next;
			}
			else {
				ret.add_elem(t2->data.str, t2->data.col, t2->data.value);
				t2 = t2->next;
			}
		}
	}
	if (t1 != nullptr && t2 != nullptr) {
		ret.add_elem(t1->data.str, t1->data.col, t1->data.value + t2->data.value);
		t1 = t1->next;
		t2 = t2->next;
		goto cycle;
	}
	else {
		if (t1 != nullptr) {
			while (t1 != nullptr) {
				ret.add_elem(t1->data.str, t1->data.col, t1->data.value);
				t1 = t1->next;
			}
		}
		else {
			while (t2 != nullptr) {
				ret.add_elem(t2->data.str, t2->data.col, t2->data.value);
				t2 = t2->next;
			}
		}
	}
	return ret;
}