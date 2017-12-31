#pragma once
#include <Windows.h>

struct chank_list
{
	chank_list* next = nullptr;
	char* chank = nullptr;
};


class SmartBytes
{
public:
	SmartBytes() : size(0), cl(nullptr)
	{
	}
	SmartBytes(DWORD s) : size(s), cl(nullptr)
	{
	}
	SmartBytes(SmartBytes& sm) : size(sm.size), cl(nullptr)
	{
		chank_list* temp1 = cl;
		chank_list* temp2 = sm.cl;
		while (temp2 != nullptr)
		{
			if (temp2->chank != nullptr)
			{
				if (temp1 == nullptr)
				{
					cl = new chank_list;
					temp1 = cl;
				}
				else
				{
					temp1->next = new chank_list;
					temp1 = temp1->next;
				}
				temp1->chank = new char[size];
				memcpy(temp1->chank, temp2->chank, size);
				//_memccpy(temp1->chank, temp2->chank, size, sizeof(char));
			}

			temp2 = temp2->next;
		}
	}
	SmartBytes& operator =(SmartBytes& sm)
	{
		size = sm.size;

		chank_list* temp1 = cl;
		while (temp1 != nullptr)
		{
			chank_list* temp2 = temp1->next;

			delete[] temp1->chank;
			delete temp1;

			temp1 = temp2;
		}
		cl = nullptr;

		temp1 = cl;
		chank_list* temp2 = sm.cl;
		while (temp2 != nullptr)
		{
			if (temp2->chank != nullptr)
			{
				if (temp1 == nullptr)
				{
					cl = new chank_list;
					temp1 = cl;
				}
				else
				{
					temp1->next = new chank_list;
					temp1 = temp1->next;
				}
				temp1->chank = new char[size];
				memcpy(temp1->chank, temp2->chank, size);
				//_memccpy(temp1->chank, temp2->chank, size, sizeof(char));
			}

			temp2 = temp2->next;
		}

		return *this;
	}

	int setSize(DWORD s)
	{
		size = s;
		return 0;
	}

	DWORD getSize()
	{
		return size;
	}

	int addChank(char* chank)
	{
		if (size == 0)
		{
			return 1;
		}

		if (cl == nullptr)
		{
			cl = new chank_list;
			cl->chank = new char[size];
			memcpy(cl->chank, chank, size);
			//_memccpy(cl->chank, chank, size, sizeof(char));
			return 0;
		}

		chank_list* temp1 = cl;
		while (temp1->next != nullptr)
		{
			temp1 = temp1->next;
		}

		temp1->next = new chank_list;
		temp1 = temp1->next;
		temp1->chank = new char[size];
		memcpy(temp1->chank, chank, size);
		//_memccpy(temp1->chank, chank, size, sizeof(char));
		return 0;
	}

	const char* getChank(int num)
	{
		chank_list* temp = cl;
		for (int i = 0; i < num; ++i)
		{
			temp = temp->next;
		}

		return temp->chank;
	}

	int setWidth(DWORD w)
	{
		width = w;
		return 0;
	}

	DWORD getWidth()
	{
		return width;
	}

	int setHeight(DWORD h)
	{
		height = h;
		return 0;
	}

	DWORD getHeight()
	{
		return height;
	}
private:
	DWORD size;
	DWORD height;
	DWORD width;
	chank_list* cl;
};

