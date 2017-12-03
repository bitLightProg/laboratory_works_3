#pragma once

#include <iostream>
#include <sstream>
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

	FileBase();
	FileBase(const char* name, int idx_count = 10, bool locked = true);
	~FileBase();

	void open(const char* name, int idx_count = 10, bool locked = true);

	/*Память указателя, возвращаемого методом p->get_data, будет освобождена в функции.
	Возвращает ключ key добавленной записи.*/
	float insert_item(InterBase* p);
	/*Побитовое копирование данных указателя void* data размером size байт в файл.
	Возвращает ключ key добавленной записи.*/
	float insert_item(void* data, int size);
	/*Поиск записи по ключу. Возвращает указатель на положение элемента в файле.*/
	std::streampos search_item(float key);
	/*Удаляет запись по ключу.*/
	void delete_item(float key);
	/*Записывает элемент, находящийся в файле на позиции pos, в память.
	Указатель dest будет указывать на выгруженный элемент.*/
	void get_item(void*& dest, std::streampos pos);
	/*Стирание удаленных записей из базы данных. Не соответсвует принципам закреплённых записей.*/
	void compress();

private:
	/*Вставка вместо удаленной записи в файле индексов. Используется в функции insert_item только при условии,
	что записи не закреплённые.*/
	float insert_on_deleted(void* data, int size);

	float* get_item(int item);
	void flush_block(int block);
	//void flush_block(std::streampos pos);
	void get_block(int block);
	void get_block(std::streampos pos);


	char* name;
	std::fstream base;
	std::fstream idx;

	bool is_locked;

	adress beg;
	adress tail;

	char* buf;
	int item_size;
	int block_size;
	int cur_block;

	int count;
	float max_key;
};

