/*Вариант  5.
Постpоить класс для pаботы с дробями. Класс должен включать соответствующие поля:
числитель, знаменатель, строковое наименование дроби (динамическое выделение памяти),
например, «три четверти» или «две пятых».

Класс должен обеспечивать пpостейшие функции для pаботы с данными классами:
сложение, вывод дроби в удобной форме.
*/
#include <iostream>
#include <cstdlib>
#include <Windows.h>

#include "Fraction.h"

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Fraction a;
	Fraction f(1, 1, "один");
	Fraction b(f);
	Fraction c(1115, 213, "тысяча сто пятнадцать двухсот тринадцатых");

	Fraction p = c.addition(b);
	p.print();
	
	c.print();
	f.print();
	a.print();
	system("pause");
	return 0;
}