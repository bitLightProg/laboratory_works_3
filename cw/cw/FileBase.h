#pragma once

#include <iostream>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <experimental\filesystem>

#include "InterBase.h"

typedef long long adress;

class FileBase
{
public:
	class Bad_Item : public std::exception {
	public:
		Bad_Item();
		Bad_Item(char const* const _Message);
	};

	class Bad_Open :public std::exception {
	public:
		Bad_Open();
		Bad_Open(char const* const _Message);
	};

	class Bad_Key :public std::exception {
	public:
		Bad_Key();
		Bad_Key(char const* const _Message);
	};

	class Bad_Pos : public std::exception {
	public:
		Bad_Pos();
		Bad_Pos(char const* const _Message);
	};

	class Critical_Error : public std::exception {
	public:
		Critical_Error();
		Critical_Error(char const* const _Message);
	};

	FileBase();
	FileBase(const char* name, int idx_count = 10);
	FileBase(const FileBase& base) = delete;
	~FileBase();

	void open(const char* name, int idx_count = 10);

	/*Память указателя, возвращаемого методом p->get_data, будет освобождена в функции.
	Возвращает ключ key добавленной записи.*/
	float insert_item(InterBase* p);
	/*Побитовое копирование данных указателя void* data размером size байт в файл.
	Возвращает ключ key добавленной записи.*/
	float insert_item(void* data, int size, float key = 0);
	/*Поиск записи по ключу. Возвращает указатель на положение элемента в файле.*/
	std::streampos search_item(float key);
	/*Удаляет запись по ключу.*/
	void delete_item(float key);
	/*Записывает элемент, находящийся в файле на позиции pos, в память.
	Возвращает указатель на выгруженный элемент.*/
	void* get_item(std::streampos pos);
	
	void print_keys();
	
private:

	float rand_key();
	static bool is_randed;
	
	void add_back_and_push_from(int block);

	int check_block(int block, float key);
	int check_item(int item, float key);
	int binary_search(int(FileBase::*p)(int item, float key), int left, int right, float key);

	float* get_item(int item);
	void flush_block(int block);
	void get_block(int block);

	char* name;
	std::fstream base;
	std::fstream idx;
	std::streampos eof;

	char* buf;
	int item_size;
	int block_size;
	int cur_block;

	int count;
};