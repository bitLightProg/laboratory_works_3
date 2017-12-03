/*
Задание   представляет   собой   типовую задачу по разработке  шаблонов стандартных структур данных.

Вариант 5
Структура данных: массив.
Способ хранения объектов: объекты.
Размерность структуры данных: параметр шаблона.
Операция: сортировка при наличии переопределенной операции
		  сравнения объектов.
Операция: Двоичный поиск на основе сравнения с объектом-ключом.
*/

#include <iostream>
#include <ctime>
#include <Windows.h>

#include "Fraction.h"
#include "Array.h"


int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	time_t t1, t2;

	Array<int, 3> arr;
	arr[0] = 0;
	arr[1] = 2;
	arr[2] = 1;
	arr.sort();
	std::cout << *(arr.find(0)) << std::endl;
	for (int i = 0; i < 3; ++i) {
		std::cout << arr[i] << std::endl;
	}
	std::cout << std::endl;
	char* str = "1234567890Hello, World!";
	Array <char, 24> arr1(str);
	arr1.sort();
	for (int i = 0; i < 24; ++i) {
		std::cout << arr1[i];
	}
	std::cout << std::endl;
	std::cout << *arr1.find('8') << std::endl;

	int int_arr[] = { 1,2,2,6,5,3 };
	Array<int, 6> arr2(int_arr);
	arr2.sort();
	std::cout << *arr2.find(3) << std::endl;

	Fraction f_arr[] = { {1,1,""}, {2,1,""}, {2,1,""}, {6,1,""}, {5,1,""},{3,1,""} };

	Array<Fraction, 6> arr_f(f_arr);
	arr_f.sort();
	arr_f.print();

	Array<int, 1000000> arr_i;
	for (int i = 0; i < 1000000; ++i) {
		arr_i[i] = rand();
	}
	t1 = clock();
	arr_i.sort();
	t2 = clock();
	std::cout << t2 - t1 << std::endl;
	system("pause");
	return 0;
}