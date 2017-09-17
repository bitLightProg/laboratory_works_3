#include <iostream>
#include <fstream>
#include <ctime>
#include <Windows.h>

void print(float **arr, int n) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cout << arr[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void write_to_file(std::ostream &os, float **arr, int n) {
	os << n << std::endl;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			os << arr[i][j] << " ";
		}
		os << std::endl;
	}
	os << std::endl;
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::cout << "Введите размерность матрицы." << std::endl;
	int n;
	std::cin >> n;
	SYSTEMTIME st1, st2;
	GetLocalTime(&st1);
	/*std::cout << "Введите элементы матрицы." << std::endl;*/
	srand(time(NULL));
	float **arr = new float*[n];
	for (int i = 0; i < n; ++i) {
		arr[i] = new float[n];
		for (int j = 0; j < n; ++j) {
			arr[i][j] = 1 + rand() % 10;
			rand() % 2 ? arr[i][j]: arr[i][j] = -arr[i][j];
		}
	}
	std::cout << std::endl;
	std::ofstream out("matrix.txt");
	write_to_file(out, arr, n);
	SYSTEMTIME s1, s2;
	GetLocalTime(&s1);
	float p = 1;
	int position = 0;
	while (position != n) {
		/*print(arr, n);*/
		p *= arr[position][position];
		for (int i = position; i < n; ++i) {
			float beg = arr[i][position];
			/*p *= beg;*/
			if (beg == 0) {
				continue;
			}
			for (int j = position; j < n; ++j) {
				arr[i][j] /= beg;
			}
		}
		
		for (int i = position + 1; i < n; ++i) {
			if (arr[i][position] == 0) {
				continue;
			}
			for (int j = position; j < n; ++j) {
				arr[i][j] -= arr[position][j];
			}
		}

		++position;
	}
	GetLocalTime(&s2);
	GetLocalTime(&st2);
	std::cout << "Определитель: "<< p << std::endl;
	std::cout << "Время: " << ((s2.wMinute - s1.wMinute) * 60 + s2.wSecond - s1.wSecond) * 1000
		+ s2.wMilliseconds - s1.wMilliseconds << std::endl;
	std::cout << "Время программы после ввода(со временем сохранения в файл): "
		<< ((st2.wMinute - st1.wMinute) * 60 + st2.wSecond - st1.wSecond) * 1000
		+ st2.wMilliseconds - st1.wMilliseconds << std::endl;
	system("pause");
	return 0;
}