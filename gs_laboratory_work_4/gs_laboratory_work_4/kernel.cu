#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>


struct BMPHeader
{
	short type;
	int size;
	short reserved1;
	short reserved2;
	int offset;
};

struct BMPInfoHeader
{
	int size;
	int width;
	int height;
	short planes;
	short bitsPerPixel;
	unsigned compression;
	unsigned imageSize;
	int xPelsPerMeter;
	int yPelsPerMeter;
	int clrUsed;
	int clrImportant;
};

BMPHeader hdr1;
BMPInfoHeader infoHdr1;
bool first_load = true;
void LoadBMPFile(uchar4 **dst, int *width, int *height, const char *name)
{

	BMPHeader hdr;
	BMPInfoHeader infoHdr;
	int x, y;

	FILE *fd;


	printf("Loading %s...\n", name);

	if (sizeof(uchar4) != 4)
	{
		printf("***Bad uchar4 size***\n");
		exit(EXIT_SUCCESS);
	}

	if (!(fd = fopen(name, "rb")))
	{
		printf("***BMP load error: file access denied***\n");
		exit(EXIT_SUCCESS);
	}

	fread(&hdr.type, sizeof(hdr.type), 1, fd);
	fread(&hdr.size, sizeof(hdr.size), 1, fd);
	fread(&hdr.reserved1, sizeof(hdr.reserved1), 1, fd);
	fread(&hdr.reserved2, sizeof(hdr.reserved2), 1, fd);
	fread(&hdr.offset, sizeof(hdr.offset), 1, fd);

	if (hdr.type != 0x4D42)
	{
		printf("***BMP load error: bad file format***\n");
		exit(EXIT_SUCCESS);
	}

	fread(&infoHdr, sizeof(infoHdr), 1, fd);

	if (infoHdr.bitsPerPixel != 24)
	{
		printf("***BMP load error: invalid color depth***\n");
		exit(EXIT_SUCCESS);
	}

	if (infoHdr.compression)
	{
		printf("***BMP load error: compressed image***\n");
		exit(EXIT_SUCCESS);
	}
	if (first_load) {
		hdr1 = hdr;
		infoHdr1 = infoHdr;
		*width = infoHdr.width;
		*height = infoHdr.height;
		first_load = false;
	}
	*dst = (uchar4 *)malloc(infoHdr.width *infoHdr.height * 4);

	printf("BMP width: %u\n", infoHdr.width);
	printf("BMP height: %u\n", infoHdr.height);

	fseek(fd, hdr.offset, SEEK_SET);

	for (y = 0; y < infoHdr.height; y++)
	{
		for (x = 0; x < infoHdr.width; x++)
		{
			(*dst)[(y * infoHdr.width + x)].z = fgetc(fd);
			(*dst)[(y * infoHdr.width + x)].y = fgetc(fd);
			(*dst)[(y * infoHdr.width + x)].x = fgetc(fd);
		}

		for (x = 0; x < (4 - (3 * infoHdr.width) % 4) % 4; x++)
			fgetc(fd);
	}


	if (ferror(fd))
	{
		printf("***Unknown BMP load error.***\n");
		free(*dst);
		exit(EXIT_SUCCESS);
	}
	else
		printf("BMP file loaded successfully!\n");

	fclose(fd);
}

constexpr int block_size = 1024;
constexpr int grid_size = 1024;

__global__ void process(uchar4* bmp1, uchar4* bmp2, uchar4* bmp3, int width, int height) {
	__shared__ int count_x;
	__shared__ int count_y;
	count_x = 1;
	count_y = 1;
	if (threadIdx.x == 0) {
		count_x = (float)width / block_size + 1;
		count_y = (float)height / grid_size + 1;
	}

	__syncthreads();

	unsigned int x = threadIdx.x;
	unsigned int y = blockIdx.x;
	
	for (x = threadIdx.x;; --count_x, x += block_size) {
		if (x >= width) {
			return;
		}
		for (y = blockIdx.x;; --count_y, y += grid_size) {
			if (y >= height) {
				break;
			}
			int offset = y*width + x;
			float sum = bmp1[offset].x + bmp1[offset].y + bmp1[offset].z;
			sum /= 3;
			
			float mult1 = sum * bmp2[offset].x / 255;
			float mult2 = sum * bmp2[offset].y / 255;
			float mult3 = sum * bmp2[offset].z / 255;
		
			bmp3[offset].x = mult1;
			bmp3[offset].y = mult2;
			bmp3[offset].z = mult3;
			bmp3[offset].w = bmp1[offset].w;
		}
	}
}

uchar4* create_bmp(uchar4* bmp1, uchar4* bmp2, int &width, int &height) {
	uchar4* bmp = new uchar4[width*height];
	uchar4* dev_bmp1 = nullptr;
	uchar4* dev_bmp2 = nullptr;
	uchar4* dev_bmp3 = nullptr;
	
	int size = width*height;

	float* sum = new float[size];

	cudaMalloc((void**)&dev_bmp1, size * sizeof(uchar4));
	cudaMalloc((void**)&dev_bmp2, size * sizeof(uchar4));
	cudaMalloc((void**)&dev_bmp3, size * sizeof(uchar4));

	cudaMemcpy(dev_bmp1, bmp1, size * sizeof(uchar4), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_bmp2, bmp2, size * sizeof(uchar4), cudaMemcpyHostToDevice);

	process << <grid_size, block_size >> > (dev_bmp1, dev_bmp2, dev_bmp3, width, height);

	cudaMemcpy(bmp, dev_bmp3, size * sizeof(uchar4), cudaMemcpyDeviceToHost);
	
	cudaFree(dev_bmp1);
	cudaFree(dev_bmp2);
	cudaFree(dev_bmp3);

	return bmp;
}

void SaveBMPFile(uchar4* uiData, const char *cName)
{
	int x, y;

	FILE *fd;

	fd = fopen(cName, "wb");

	fwrite(&hdr1.type, sizeof(hdr1.type), 1, fd);
	fwrite(&hdr1.size, sizeof(hdr1.size), 1, fd);
	fwrite(&hdr1.reserved1, sizeof(hdr1.reserved1), 1, fd);
	fwrite(&hdr1.reserved2, sizeof(hdr1.reserved2), 1, fd);
	fwrite(&hdr1.offset, sizeof(hdr1.offset), 1, fd);
	fwrite(&infoHdr1, sizeof(infoHdr1), 1, fd);
	
	fseek(fd, hdr1.offset, SEEK_SET);

	for (y = 0; y < infoHdr1.height; y++)
	{
		for (x = 0; x < infoHdr1.width; x++)
		{
			fputc(((uchar4*)uiData)[(y * infoHdr1.width + x)].z, fd);
			fputc(((uchar4*)uiData)[(y * infoHdr1.width + x)].y, fd);
			fputc(((uchar4*)uiData)[(y * infoHdr1.width + x)].x, fd);
		}

		for (x = 0; x < (4 - (3 * infoHdr1.width) % 4) % 4; x++)
		{
			fputc(' ', fd);
		}
	}
	printf("Image %s is saved.\n", cName);

	fclose(fd);
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	uchar4* bmp1 = nullptr;
	uchar4* bmp2 = nullptr;
	uchar4* bmp3 = nullptr;
	int width = 0;
	int height = 0;
	while (true) {
		std::cout << "Все картинки должны быть в формате .bmp" << std::endl;
		std::cout << "1. Ввести адрес первой картинки." << std::endl
			<< "2. Ввести адрес второй картинки." << std::endl
			<< "3. Получить результирующую картинку." << std::endl
			<< "0. Завершение программы." << std::endl;

		int choose;
		std::cin >> choose;
		if (choose == 1) {
			std::cout << "Введите адрес первой картинки." << std::endl;
			std::string str;
			getchar();
			std::getline(std::cin, str);
			int* width1 = new int;
			int* height1 = new int;
			
			LoadBMPFile(&(bmp1), width1, height1, str.c_str());
			width = *width1;
			height = *height1;
		}
		else if (choose == 2) {
			std::cout << "Введите адрес второй картинки." << std::endl;
			std::string str;
			getchar();
			std::getline(std::cin, str);
			int* width1 = new int;
			int* height1 = new int;
			LoadBMPFile(&bmp2, width1, height1, str.c_str());
		}
		else if (choose == 3) {
			std::cout << "Введите адрес результирующей картинки." << std::endl;
			std::string str;
			getchar();
			std::getline(std::cin, str);
			bmp3 = create_bmp(bmp1, bmp2, width, height);
			SaveBMPFile(bmp3, str.c_str());
		}
		else if (choose == 0) {
			break;
		}
	}
	return 0;
}
