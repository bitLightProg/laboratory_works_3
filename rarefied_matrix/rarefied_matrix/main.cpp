#include <iostream>
#include "Rarefied_Matrix.h"

int main() {
	Rarefied_Matrix m1, m2;
	int n = 5;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			int k = rand() % 2;
			m1.add_elem(i, j, k);
		}
	}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			int k = rand() % 2;
			m2.add_elem(i, j, k);
		}
	}
	m1.print();
	std::cout << std::endl;
	m2.print();
	std::cout << std::endl;
	/*m1.add_elem(0, 0, 1);
	m1.add_elem(0, 1, 1);
	m2.add_elem(0, 0, 3);
	m2.add_elem(0, 2, 1);*/
	Rarefied_Matrix m3 = m1 + m2;
	m3.print();

	system("pause");
	return 0;
}