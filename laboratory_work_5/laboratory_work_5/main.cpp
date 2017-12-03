﻿/*Вариант  5.
Постpоить класс для pаботы с дробями. Класс должен включать соответствующие поля:
числитель, знаменатель, строковое наименование дроби (динамическое выделение памяти),
например, «три четверти» или «две пятых».

Класс должен обеспечивать пpостейшие функции для pаботы с данными классами:
сложение, вывод дроби в удобной форме.

Для класса из лаб. работы №1 реализовать набор операций для работы с объектами класса:
сложение (как метод класса), вычитание (как дружественную функцию), инкремент постфиксный
и префиксный, преобразование к разным типам (по усмотрению студентов и преподавателя).

Для класса из лаб. работы №2 перегрузить операции ввода/вывода, позволяющие осуществлять
ввод и вывод в удобной фоpме объектов классов:
•	вывод в текстовый файл;
•	ввод/вывод из двоичного файла / в двоичный файл.

Для классов предыдущей лабораторной работы реализовать иерархию, изменяя отдельные методы
и добавляя члены-данные (по усмотрению студента и преподавателя).
В иерархию должно входить 2 производных класса. Один из методов должен быть виртуальным.

Реализовать с помощью классов динамическую списочную структуру, содержащую объекты классов,
связанных наследованием. В классах реализовать методы добавления, удаления, вставки по номеру,
удаления по номеру, поиска и просмотр всей структуры.

Вариант 5
Структура данных: дек с ограниченным выходом (добавление с двух концов, удаление с одного конца),
реализованный на однонаправленном списке. Способ хранения объектов: объекты.

*/
#include <iostream>
#include <Windows.h>

#include "Fraction.h"
#include "CeilFraction.h"
#include "FloorFraction.h"
#include "Deq.h"

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	CeilFraction a;
	CeilFraction b(1513, 217, "тысяча пятьсот тринадцать двухсот семнадцатых");
	FloorFraction c;
	FloorFraction d(1513, 217, "тысяча пятьсот тринадцать двухсот семнадцатых");
	FloorFraction e(2, 1, "два");

	Deq deqq(b, d);
	
	deqq.push_forward(e);
	deqq.print_all();
	deqq.pop_forward();
	deqq.push_back(e);
	deqq.print_all();

	deqq.search(0, 0);
	deqq.search(2, 1)->print();

	system("pause");
	return 0;
}