/*
Для встроенного типа и класса из лабораторной работы №1 провести временной анализ заданных шаблонных классов
на основных операциях: добавление, удаление, поиск, сортировка. Использовать итераторы для работы с контейнерами.
Для получения времени выполнения операции засекать системное время начала и окончания операции
и автоматически генерировать большое количество данных.

Вариант 5
	очередь;
	словарь.
*/

#include <iostream>
#include <ctime>
#include <Windows.h>
#include <queue>
#include <map>

#include "Fraction.h"

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::queue<int> i_q;
	time_t t1, t2;
	// Поэлементное добавление.
	t1 = clock();
	for (int i = 0; i < 1000000; ++i) {
		i_q.push(i);
	}
	t2 = clock();
	std::cout << "Поэлементное добавление int в очередь." << std::endl;
	std::cout << t2 - t1 << std::endl;
	// Поэлементное удаление.
	t1 = clock();
	for (int i = 0; i < 1000000; ++i) {
		i_q.pop();
	}
	t2 = clock();
	std::cout << "Поэлементное удаление из очереди." << std::endl;
	std::cout << t2 - t1 << std::endl;

	std::map<int, int> i_m;
	std::map<int, int>::iterator m_it1, m_it2;
	t1 = clock();
	for (int i = 0; i < 1000000; ++i) {
		i_m[i] = i+1;
	}
	t2 = clock();
	std::cout << "Поэлементное добавление int в словарь." << std::endl;
	std::cout << t2 - t1 << std::endl;
	m_it1 = i_m.begin();
	for (; m_it1 != i_m.end(); ++m_it1) {
		//std::cout << m_it1->first << " " << m_it1->second << std::endl;
	}
	t1 = clock();
	// что находится на этом месте. исключение при несуществующем ключе
	std::cout << i_m.at(21) << std::endl;
	t2 = clock();
	std::cout << t2 - t1 << std::endl;
	t1 = clock();
	try {
		std::cout << i_m.at(-1) << std::endl;
	}
	catch (std::exception ex) {
		std::cout << ex.what() << std::endl;
	}
	t2 = clock();
	std::cout << t2 - t1 << std::endl;
	t1 = clock();
	// поиск по ключу
	m_it2 = i_m.find(999999);
	std::cout << m_it2->first << " " << m_it2->second << std::endl;
	t2 = clock();
	std::cout << t2 - t1 << std::endl;

	t1 = clock();
	i_m.erase(i_m.begin(), prev(i_m.end()));
	t2 = clock();
	std::cout << t2 - t1 << std::endl;

	std::map<Fraction, int> f_m;
	t1 = clock();
	for (int i = 0; i < 1000; ++i) {
		f_m[Fraction(i, 2, "")] = i;
	}
	t2 = clock();
	std::cout << t2 - t1 << std::endl;

	std::map<Fraction, int>::iterator f_it;
	f_it = f_m.find(Fraction(333, 2, ""));
	std::cout << f_it->first << " = " << f_it->second << std::endl;

	t1 = clock();
	f_m.clear();
	t2 = clock();
	std::cout << t2 - t1 << std::endl;

	system("pause");
	return 0;
}