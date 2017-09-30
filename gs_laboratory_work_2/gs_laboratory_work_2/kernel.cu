
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <iostream>
#include <ctime>
#include <Windows.h>

//cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size);
//
//__global__ void addKernel(int *c, const int *a, const int *b)
//{
//    int i = threadIdx.x;
//    c[i] = a[i] + b[i];
//}
//
//int main()
//{
//    const int arraySize = 5;
//    const int a[arraySize] = { 1, 2, 3, 4, 5 };
//    const int b[arraySize] = { 10, 20, 30, 40, 50 };
//    int c[arraySize] = { 0 };
//
//    // Add vectors in parallel.
//    cudaError_t cudaStatus = addWithCuda(c, a, b, arraySize);
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "addWithCuda failed!");
//        return 1;
//    }
//
//    printf("{1,2,3,4,5} + {10,20,30,40,50} = {%d,%d,%d,%d,%d}\n",
//        c[0], c[1], c[2], c[3], c[4]);
//
//    // cudaDeviceReset must be called before exiting in order for profiling and
//    // tracing tools such as Nsight and Visual Profiler to show complete traces.
//    cudaStatus = cudaDeviceReset();
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaDeviceReset failed!");
//        return 1;
//    }
//	system("pause");
//    return 0;
//}
//
//// Helper function for using CUDA to add vectors in parallel.
//cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size)
//{
//    int *dev_a = 0;
//    int *dev_b = 0;
//    int *dev_c = 0;
//    cudaError_t cudaStatus;
//
//    // Choose which GPU to run on, change this on a multi-GPU system.
//    cudaStatus = cudaSetDevice(0);
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
//        goto Error;
//    }
//
//    // Allocate GPU buffers for three vectors (two input, one output)    .
//    cudaStatus = cudaMalloc((void**)&dev_c, size * sizeof(int));
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaMalloc failed!");
//        goto Error;
//    }
//
//    cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaMalloc failed!");
//        goto Error;
//    }
//
//    cudaStatus = cudaMalloc((void**)&dev_b, size * sizeof(int));
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaMalloc failed!");
//        goto Error;
//    }
//
//    // Copy input vectors from host memory to GPU buffers.
//    cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaMemcpy failed!");
//        goto Error;
//    }
//
//    cudaStatus = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaMemcpy failed!");
//        goto Error;
//    }
//
//    // Launch a kernel on the GPU with one thread for each element.
//    addKernel<<<1, size>>>(dev_c, dev_a, dev_b);
//
//    // Check for any errors launching the kernel
//    cudaStatus = cudaGetLastError();
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
//        goto Error;
//    }
//    
//    // cudaDeviceSynchronize waits for the kernel to finish, and returns
//    // any errors encountered during the launch.
//    cudaStatus = cudaDeviceSynchronize();
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
//        goto Error;
//    }
//
//    // Copy output vector from GPU buffer to host memory.
//    cudaStatus = cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaMemcpy failed!");
//        goto Error;
//    }
//
//Error:
//    cudaFree(dev_c);
//    cudaFree(dev_a);
//    cudaFree(dev_b);
//    
//    return cudaStatus;
//}

void print(float* arr, int n) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cout << arr[i*n + j] << " ";
		}
		std::cout << std::endl;
	}
	/*std::cout << std::endl;*/
}

//void rutine(float* arr, int n, int& position, float& p);

__device__ unsigned int block_id = -1;
__global__ void div(float* arr, int n, int position, float* p) {
	__shared__ float beg;
	beg = 0;
	if (threadIdx.x == 0) {
		beg = arr[(n)*(blockIdx.x + position) + position];
		/*if (beg == 3)
			return;*/
		/*__threadfence();*/
		
		int current_index = blockIdx.x;
		if (beg == 0 && current_index != 0)
			return;
		while (block_id != current_index - 1);
		*p *= beg;
		++block_id;
		/*__threadfence();*/
	}
	/*__syncthreads();*/
	if (beg == 0) {
		return;
	}
	unsigned int x = 1024*1024*(threadIdx.z) + 1024*(threadIdx.y) + (threadIdx.x) + position;
	unsigned int y = blockIdx.x + position;
	
	if ((x >= n) || (y >= n)) {
		return;
	}
	///*arr[y*n + x] = y + position;*/
	//if (x == 0) {
	//	beg = arr[(n)*y + position];
	//	if (beg == 0) {
	//		return;
	//	}
	//	*p *= beg;
	//}
	/*__syncthreads();*/
	/*if (beg != 0) {*/
		arr[y*(n)+x] /= beg;
	/*}*/


}
__global__ void sub(float* arr, int n, int position) {
	block_id = -1;
	__shared__ float beg;
	beg = 0;
	if (threadIdx.x == 0) {
		beg = arr[(n)*(blockIdx.x + position) + position];
	}
	/*unsigned int x = blockIdx.x + position;
	unsigned int y = 1024 * 1024 * (threadIdx.z) + 1024 * (threadIdx.y) + (threadIdx.x) + position;
	if (x >= n || y >= n) {
		return;
	}

	arr[y*(n)+x] -= arr[x + position*n];*/
	unsigned int x = 1024 * 1024 * (threadIdx.z) + 1024 * (threadIdx.y) + (threadIdx.x) + position;
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

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::cout << "Введите размерность матрицы." << std::endl;
	int n;
	std::cin >> n;
	std::cout << "Введите элементы матрицы." << std::endl;
	srand(time(NULL));
	float *arr = new float[n*n];
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cin >> arr[i*n + j];
		}
	}
	std::cout << std::endl;

	float* dev_arr = nullptr;
	float* dev_p = nullptr;
	float p = 1;
	int size = n*n;
	
	int position = 0;

	cudaMalloc((void**)&dev_arr, size * sizeof(float));
	cudaMalloc((void**)&dev_p, sizeof(float));

	cudaMemcpy(dev_p, &p, sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_arr, arr, size * sizeof(float), cudaMemcpyHostToDevice);

	SYSTEMTIME s1, s2;
	GetLocalTime(&s1);
	while (position != n) {
		dim3 block(1 + (n - position) % 1024, 1/*1 + ((int)(n-position) / 1024 % 1024)*/, 1/*1 + (int)(n - position) / 1024 / 1024*/);
		dim3 grid((n - position), 1, 1);
		div<<<grid, block>>>(dev_arr, n, position, dev_p);
		/*cudaMemcpy(arr, dev_arr, size * sizeof(float), cudaMemcpyDeviceToHost);
		print(arr, n);
		cudaMemcpy(&p, dev_p, sizeof(float), cudaMemcpyDeviceToHost);
		std::cout << p << std::endl << std::endl;*/
		sub<<<grid, block>>>(dev_arr, n, position);
		/*cudaMemcpy(arr, dev_arr, size * sizeof(float), cudaMemcpyDeviceToHost);
		print(arr, n);
		cudaMemcpy(&p, dev_p, sizeof(float), cudaMemcpyDeviceToHost);
		std::cout << p << std::endl << std::endl;*/
		/*print(arr, n);
		rutine(arr, n, position, p);*/
		++position;
		/*cudaMemcpy(dev_pos, &position, sizeof(float), cudaMemcpyHostToDevice);*/
	}
	cudaMemcpy(&p, dev_p, sizeof(float), cudaMemcpyDeviceToHost);
	GetLocalTime(&s2);
	cudaFree(dev_arr);
	cudaFree(dev_p);
	
	std::cout << "Определитель: " << p << std::endl;
	std::cout << "Время: " << ((s2.wMinute - s1.wMinute) * 60 + s2.wSecond - s1.wSecond) * 1000
		+ s2.wMilliseconds - s1.wMilliseconds << std::endl;
	system("pause");
	return 0;
}

//void rutine(float* arr, int n, int& position) {
//	for (int i = position; i < n; ++i) {
//		float beg = arr[i*n + position];
//		p *= beg;
//		if (beg == 0) {
//			continue;
//		}
//		for (int j = position; j < n; ++j) {
//			arr[i*n + j] /= beg;
//		}
//	}
//
//	for (int i = position + 1; i < n; ++i) {
//		if (arr[i*n + position] == 0) {
//			continue;
//		}
//		for (int j = position; j < n; ++j) {
//			arr[i*n + j] -= arr[position*n + j];
//		}
//	}
//
//	++position;
//}