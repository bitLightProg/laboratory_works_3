/*Вариант  5.
Постpоить класс для pаботы с дробями. Класс должен включать соответствующие поля:
числитель, знаменатель, строковое наименование дроби (динамическое выделение памяти),
например, «три четверти» или «две пятых».

Класс должен обеспечивать пpостейшие функции для pаботы с данными классами:
сложение, вывод дроби в удобной форме.
*/
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <Windows.h>

#include "Fraction.h"

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Fraction a;
	Fraction f(1, 1, "один");
	Fraction b(f);
	Fraction c(1115, 213, "тысяча сто пятнадцать двухсот тринадцатых");

	/*std::ofstream output("out.txt");

	output << c;
	output.close();
	std::ifstream input("out.txt");
	input >> a;
	output.open("out2.txt");
	output << a;
	output.close();*/

	std::ofstream output_bin("out.bin", std::ios_base::binary);
	c.write(output_bin);
	output_bin.close();
	std::ifstream input_bin("out.bin", std::ios_base::binary);
	a.read(input_bin);
	std::cout << a << std::endl;
	system("pause");
	return 0;
}