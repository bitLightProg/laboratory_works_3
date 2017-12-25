/*
8. Реализовать АТД «Файловая структура данных» в соответствии с вариантом. (начальный балл  5)
Операции АТД:
Insert – вставка записи в файл,
Delete – удаление записи из файла,
Search - поиск записи в файле.

1.	Элементом данных, хранящихся в файле, является запись, имеющая уникальное ключевое значение.
2.	Запись в файле представлена индексом, т.е. парой (k,p), где k – ключевое значение,
	p – файловый указатель на начало записи в файле.
3.	Для файла данных поддерживается файловая структура индексов, указанная в варианте задания.
4.	Файл индексов имеет страничную структуру. Страницы содержат индексы записей и имеют фиксированный размер.
5.	Чтение и запись в файл индексов ведется постранично.
6.	Тестирование файловой структуры ведется для различных значений параметров:
	N – число записей в файле данных, N = 10^3, 10^4, 10^5, 10^6,
	M – число индексов на странице файла индексов, M = 10,100, 1000.
7.	Число обращений к блокам файла индексов в процессе выполнения операций должно соответствовать:
	для хешированного файла  - N/(M*K), где K – число сегментов хеш – таблицы,
	для индексированного файла – 2 + log(N/M),
	для B- дерева файла – 2 + logt (N/M), где t – степень В – дерева.

4.	АТД «Плотный индекс файла». Записи закрепленные. Ключ записи - вещественное число.
*/

#include <Windows.h>
#include "FileBase.h"
#include "Fraction.h"

int main() {
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	std::ofstream ("my_base.bin");
	std::ofstream ("idx_my_base.bin");
	int b = 10;
	int n = 10;
	std::cout << "Введите количество записей в блоке." << std::endl;
	std::cin >> n;
	FileBase a("my_base.bin", n);

	time_t time1, time2;

	char ch[256];
	float* keys = nullptr;
	int N;
	for (int i = 0; i < 256; ++i) {
		ch[i] = 48 + rand() % 10;
	}
	while (true) {
		system("cls");
		std::cout << "Количество записей в блоке файла индексов: " << n << std::endl;
		std::cout << "1. Добавить n случайных записей." << std::endl
			<< "2. Ввести свой ключ." << std::endl
			<< "3. Удалить ключ." << std::endl
			<< "4. Вывести ключи." << std::endl
			<< "5. Добавить свои данные по своему ключу(пример: 1.23e8 данные)." << std::endl
			<< "6. Получить данные из базы(не рекомендуется для случайно сгенерированных данных)." << std::endl
			<< "7. Найти n случайных записей(для установления времени поиска)." << std::endl
			<< "0. Завершение работы программы." << std::endl;
		int choose;
		std::cin >> choose;
		if (choose == 1) {
			std::cout << "Введите n." << std::endl;
			int n = 0;
			std::cin >> n;
			if (keys) {
				delete[] keys;
				keys = nullptr;
			}
			keys = new float[n];
			N = n;
			float key = 0;
			int sz = 20;
			time1 = clock();
			for (int i = 0; i < n; ++i) {
				try {
					keys[i] = a.insert_item(ch, sz);
				}
				catch (std::exception &ex) {
					std::cout << ex.what() << std::endl;
					system("pause");
					continue;
				}
			}
			time2 = clock();
			std::cout << "Данные успешно добавлены." << std::endl;
			std::cout << "Время, за которое данные были добавлены: " << time2 - time1 << " мс." << std::endl;
			system("pause");
		}
		else if (choose == 2) {
			std::cout << "Введите ключ." << std::endl;
			float key;
			std::cin >> key;
			a.insert_item(ch, 20, key);
			std::cout << "Ключ успешно добавлен." << std::endl;
			system("pause");
		}
		else if (choose == 3) {
			std::cout << "Введите ключ." << std::endl;
			float key;
			std::cin >> key;
			try {
				a.delete_item(key);
			}
			catch (std::exception &ex) {
				std::cout << ex.what() << std::endl;
				system("pause");
				continue;
			}
			std::cout << "Данные успешно удалены." << std::endl;
			system("pause");
		}
		else if (choose == 4) {
			a.print_keys();
			system("pause");
		}
		else if (choose == 5) {
			std::cout << "Введите ключ и данные." << std::endl;
			float key;
			std::cin >> key;
			char str[256];
			int i = 0;
			getchar();
			while (str[i++] = getchar()) {
				if (str[i - 1] == '\n') {
					str[i - 1] = '\0';
					break;
				}
			}
			try {
				a.insert_item(str, i, key);
			}
			catch (std::exception &ex) {
				std::cout << ex.what() << std::endl;
				system("pause");
				continue;
			}
			std::cout << "Данные успешно добавлены." << std::endl;
			system("pause");
		}
		else if (choose == 6) {
			std::cout << "Введите ключ." << std::endl;
			float key;
			std::cin >> key;
			char* str;
			try {
				str = reinterpret_cast<char*>(a.get_item(a.search_item(key)));
			}
			catch (std::exception &ex) {
				std::cout << ex.what() << std::endl;
				system("pause");
				continue;
			}
			std::cout << str << std::endl;
			system("pause");
		}
		else if (choose == 7) {
			if (!keys) {
				std::cout << "Ключи не были сгенерированы." << std::endl;
				system("pause");
				continue;
			}
			std::cout << "Введите n." << std::endl;
			int n = 0;
			std::cin >> n;
			time1 = clock();
			for (int i = 0; i < n; ++i) {
				int k = rand() % N;
				try {
					a.search_item(keys[k]);
				}
				catch (std::exception &ex) {
					std::cout << ex.what() << std::endl;
					system("pause");
					continue;
				}
			}
			time2 = clock();
			std::cout << "Время поиска n элементов: " << time2 - time1 << " мс." << std::endl;
			system("pause");
		}
		else if (choose == 0)
			break;
	}

	
	system("pause");
	return 0;
}