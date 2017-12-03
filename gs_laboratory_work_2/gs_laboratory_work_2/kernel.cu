
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <Windows.h>

__device__ unsigned int block_id = -1;


__global__ void div(float* arr, int n, int position, float* p) {
	__shared__ float beg;
	beg = arr[(n)*(blockIdx.x + position) + position];
	if (threadIdx.x == 0) {
		beg = arr[(n)*(blockIdx.x + position) + position];

		int current_index = blockIdx.x;
		if (beg == 0 && current_index != 0)
			return;
		while (block_id != current_index - 1);
		*p *= beg;
		++block_id;
	}

	unsigned int x = threadIdx.x + position;
	unsigned int y = blockIdx.x + position;
	
	if ((x >= n) || (y >= n)) {
		return;
	}
	
	if (beg != 0) {
		arr[y*(n)+x] /= beg;
	}
}
__global__ void sub(float* arr, int n, int position) {
	block_id = -1;
	__shared__ float beg;
	beg = 0;
	if (threadIdx.x == 0) {
		beg = arr[(n)*(blockIdx.x + position) + position];
	}
	
	unsigned int x = threadIdx.x + position;
	unsigned int y = blockIdx.x + position;
	__syncthreads();
	if ((x >= n) || (y >= n)) {
		return;
	}
	if (beg == 0) {
		return;
	}
	arr[y*(n)+x] -= arr[n*position + x];

}

void read_from_file(std::ifstream& input, int& n, float*& arr) {
	input.read((char*)&n, sizeof(int));
	arr = new float[n*n];
	input.read((char*)arr, n*n * sizeof(float));
}

void write_to_file(std::ostream &os, float*& arr, int& n) {
	os.write((char*)&n, sizeof(int));
	os.write((char*)arr, n*n * sizeof(float));
}

void rutine(float*& arr, int& n, int& position, float& p) {
	for (int i = position; i < n; ++i) {
		float beg = arr[i*n + position];
		if (beg == 0 && i != position)
			continue;
		p *= beg;

		for (int j = position; j < n; ++j) {
			arr[i*n + j] /= beg;
		}
	}

	for (int i = position + 1; i < n; ++i) {
		if (arr[i*n + position] == 0) {
			continue;
		}
		for (int j = position; j < n; ++j) {
			arr[i*n + j] -= arr[position*n + j];
		}
	}

	++position;
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::cout << "1. Ввести с клавиатуры." << std::endl;
	std::cout << "2. Читать из бинарного файла." << std::endl;
	std::cout << "3. Комплексное тестирование." << std::endl;
	int chose;
	std::cin >> chose;
	int n;
	float *arr = nullptr;
	int position = 0;
	float p = 1;

	if (chose == 1) {
		std::cout << "Введите размерность матрицы." << std::endl;
		
		std::cin >> n;
		std::cout << "Введите элементы матрицы." << std::endl;
		srand(time(NULL));
		arr = new float[n*n];
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				std::cin >> arr[i*n + j];
			}
		}
		std::cout << std::endl;
	}
	else if (chose == 2) {
		std::ifstream in_bin("input.bin", std::ios_base::binary);
		read_from_file(in_bin, n, arr);
	}
	else if (chose == 3) {
		std::cout << "Введите размерность матрицы." << std::endl;
		std::cin >> n;
		arr = new float[n*n];
		srand(time(NULL));
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				arr[i*n + j] = 1 + rand() % 10;
				rand() % 2 ? arr[i*n + j] : arr[i*n + j] = -arr[i*n + j];
			}
		}
		std::ofstream out("input.bin", std::ios_base::binary);
		write_to_file(out, arr, n);
		SYSTEMTIME s1, s2;
		GetLocalTime(&s1);
		while (position != n) {
			rutine(arr, n, position, p);
		}
		GetLocalTime(&s2);
		std::cout << "Определитель: " << p << std::endl;
		std::cout << "Время: " << (((s2.wHour - s1.wHour)*60 + s2.wMinute - s1.wMinute) * 60 + s2.wSecond - s1.wSecond) * 1000
			+ s2.wMilliseconds - s1.wMilliseconds << std::endl;
	}
	else return 0;

	float* dev_arr = nullptr;
	float* dev_p = nullptr;
	int size = n*n;
	position = 0;
	float k = p;
	p = 1;
	if (chose == 3) {
		std::ifstream in_bin("input.bin", std::ios_base::binary);
		read_from_file(in_bin, n, arr);
		in_bin.close();
	}	

	cudaMalloc((void**)&dev_arr, size * sizeof(float));
	cudaMalloc((void**)&dev_p, sizeof(float));

	cudaMemcpy(dev_p, &p, sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_arr, arr, size * sizeof(float), cudaMemcpyHostToDevice);

	SYSTEMTIME s1, s2;
	GetLocalTime(&s1);
	while (position != n) {
		div<<<n-position, n - position >>>(dev_arr, n, position, dev_p);		
		sub<<<n - position, n - position >>>(dev_arr, n, position);
		
		++position;
	}
	cudaMemcpy(&p, dev_p, sizeof(float), cudaMemcpyDeviceToHost);
	GetLocalTime(&s2);
	
	std::cout << "Определитель: " << p << std::endl;
	std::cout << "Время: " << ((s2.wMinute - s1.wMinute) * 60 + s2.wSecond - s1.wSecond) * 1000
		+ s2.wMilliseconds - s1.wMilliseconds << std::endl;
	system("pause");
	cudaFree(dev_arr);
	cudaFree(dev_p);
	delete[] arr;
	return 0;
}

