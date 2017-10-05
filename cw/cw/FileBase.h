#pragma once

#include <fstream>

#include "InterBase.h"


class FileBase
{
public:
	FileBase();
	~FileBase();

	/*Память указателя InterBase* p будет освобождена в функции.
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

private:
	std::fstream file;
};

