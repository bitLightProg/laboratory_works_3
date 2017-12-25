#define _CRT_SECURE_NO_WARNINGS
#include "FileBase.h"

bool FileBase::is_randed = false;

FileBase::Bad_Item::Bad_Item() : std::exception() {}
FileBase::Bad_Item::Bad_Item(char const* const _Message) : std::exception(_Message) {}
FileBase::Bad_Open::Bad_Open() :std::exception() {}
FileBase::Bad_Open::Bad_Open(char const* const _Message): std::exception(_Message) {}
FileBase::Bad_Key::Bad_Key() : std::exception() {}
FileBase::Bad_Key::Bad_Key(char const* const _Message) : std::exception(_Message) {}
FileBase::Bad_Pos::Bad_Pos() : std::exception() {}
FileBase::Bad_Pos::Bad_Pos(char const* const _Message) : std::exception(_Message) {}
FileBase::Critical_Error::Critical_Error() : std::exception() {}
FileBase::Critical_Error::Critical_Error(char const* const _Message) : std::exception(_Message) {}


FileBase::FileBase() :item_size(0), block_size(0), cur_block(-1), buf(nullptr), name(nullptr)/*, is_locked(true)*/ {
}

FileBase::FileBase(const char* name, int idx_count):FileBase() {
	open(name, idx_count);
}

FileBase::~FileBase() {
	flush_block(cur_block);
	if (base.is_open()) {
		base.close();
	}
	if (idx.is_open()) {
		idx.close();
	}
	if (name) {
		delete[] name;
		name = nullptr;
	}
	if (buf) {
		delete[] buf;
		buf = nullptr;
	}
}

void FileBase::open(const char* name, int idx_count) {
	if (!is_randed) {
		srand(time(NULL));
		is_randed = true;
	}

	count = idx_count;
	int mode = std::ios::in | std::ios::out | std::ios::binary | std::ios::ate;

	int ln = 4 + strlen(name) + 1;
	this->name = new char[ln-4];
	strcpy(this->name, name);
	char* temp = new char[ln];
	memcpy(temp, "idx_", 5);
	strcat(temp, name);

	try {
		base.open(name, mode);
		idx.open(temp, mode);
		if (!base.is_open() || !idx.is_open())
			throw Bad_Open("Couldn't open the base.");
	}
	catch (std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		delete[] temp;
		throw;
	}
	/*Получение данных из существующей базы.*/
	idx.seekg(0, std::ios::end);
	if (idx.tellg() != std::streampos(0)) {
		eof = idx.tellg();

		idx.seekg(0);
		idx.read(reinterpret_cast<char*>(&count), sizeof(int));

		item_size = sizeof(float) + sizeof(adress);
		block_size = item_size*count;

		buf = new char[block_size];
	}
	else {
		eof = idx.tellg();
		item_size = sizeof(float) + sizeof(adress);
		block_size = item_size*count;
		buf = new char[block_size];
	}
	delete[] temp;
}

float FileBase::insert_item(void* data, int size, float hand_key) {
	if ((adress)eof == -1) {
		throw Critical_Error("Data corruption.");
	}
	if (!idx.is_open() || !base.is_open()) {
		throw Bad_Open("Base isn't opened.");
	}

regenerate:

	/*TODO: Сгенерировать ключ.*/
	float key = rand_key();
	if (hand_key != 0)
		key = hand_key;

	/*TODO: Найти место для вставки.*/

	std::streampos left(sizeof(int));
	idx.seekg(0, std::ios::end);

	/*Первая вставка.*/
	if ((adress)eof == 0) {
		eof = idx.tellg();
	}

	/*Если база пустая.*/
	if ((adress)eof == 0) {
		idx.write(reinterpret_cast<char*>(&count), sizeof(count));
		idx.seekg(0, std::ios::end);
		eof = idx.tellg();
	}
	std::streampos right = idx.tellg();

	int mid_block;
	if (left == right) {
		mid_block = 0;//GOTO: Вставка в блок mid_block.
		goto insert;
	}

	int left_block = 0;
	int right_block = ((adress)right - sizeof(int)) / block_size;
	/*Находим блок для вставки.*/
	mid_block = binary_search(&FileBase::check_block, left_block, right_block, key);

insert:
	/*TODO: Вставить.*/
	/*Если вставка не в текущий блок.*/
	if (cur_block != mid_block) {
		flush_block(cur_block);
		get_block(mid_block);
		cur_block = mid_block;
	}

	float* item = get_item(count - 1);
	char* temp = nullptr;
	bool is_overflow = false;
	/*Если блок заполнен.*/
	if (*item != 0) {
		/*Если ключ больше самого последнего ключа.*/
		if (check_item(count - 1, key) > 0) {
			++mid_block;
			goto insert;
		}
		is_overflow = true;//Переполнение.
		temp = new char[item_size];
		memcpy(temp, item, item_size);
	}

	int left_item, right_item;
	left_item = 0;
	right_item = count - 1;
	int mid_item = binary_search(&FileBase::check_item, left_item, right_item, key);
	if (check_item(mid_item, key) == 0) {
		hand_key = 0;
		goto regenerate; // Если такой ключ уже есть.
	}

	int place_to_insert = 0;
	int check = check_item(mid_item, 0);
	/*Если это свободное место.*/
	if (check == 0) {
		place_to_insert = mid_item;
	}
	else {
		/*Ищем свободное место.*/
		for (;;) {
			int check = check_item(mid_item, key);
			if (check > 0) {
				++mid_item;
			}
			else {
				place_to_insert = mid_item;
				/*Сдвиг элементов вправо.*/
				int move_place = count - 1;
				for (int i = (place_to_insert + 1)*item_size; i < block_size; i += item_size) {
					if (*reinterpret_cast<float*>(buf + i) == 0) {
						move_place = i / item_size;
						break;
					}
				}
				for (int i = (move_place + 1)*item_size - 1; i >= (place_to_insert + 1)*item_size; --i) {
					buf[i] = buf[i - item_size];
				}
				break;
			}
		}
	}

	item = get_item(place_to_insert);
	*item = key;
	item++;

	/*Запись в базу данных и сохранение индекса.*/
	base.write(reinterpret_cast<char*>(&size), sizeof(int));
	*((adress*)item) = base.tellp();
	base.write(reinterpret_cast<char*>(data), size);


	/*TODO: Перенести, если переполнение.*/
	if (is_overflow) {
		int cnt = ceil((float)count / 2);
		int l_cnt = count - cnt;
		char* moved = new char[item_size*cnt];
		/*Копирование из старого блока.*/
		memcpy(moved, buf + item_size*(l_cnt + 1), item_size*(cnt - 1));
		/*Копирование последнего элемента, сохранённого ранее.*/
		memcpy(moved + item_size*(cnt - 1), temp, item_size);
		/*Зануление копируемых элементов.*/
		for (int i = item_size*(l_cnt + 1); i < block_size; ++i) {
			buf[i] = 0;
		}
		int block_pos = cur_block;
		/*Сдвиг всех блоков.*/
		add_back_and_push_from(cur_block + 1);
		/*Опустошение буфера.*/
		for (int i = 0; i < block_size; ++i) {
			buf[i] = 0;
		}
		memcpy(buf, moved, item_size*cnt);

		delete[] moved;
		delete[] temp;

		/*Запись в качестве следующего блока.*/
		flush_block(block_pos + 1);
		cur_block = block_pos + 1;

	}
	return key;
}

float FileBase::insert_item(InterBase* p) {
	void* data = p->get_data();
	try {		
		float result = insert_item(data, p->size_of_data());
		delete[] data;
		return result;
	}
	catch (std::exception &ex) {
		delete[] data;
		throw;
	}
}

std::streampos FileBase::search_item(float key) {
	int last_block = ((adress)eof - sizeof(int)) / block_size;
	int mid_block = binary_search(&FileBase::check_block, 0, last_block, key);
	/*Считывание блока.*/
	if (mid_block != cur_block) {
		flush_block(cur_block);
		get_block(mid_block);
	}
	int mid = binary_search(&FileBase::check_item, 0, count - 1, key);
	/*Если элемент найден.*/
	if (check_item(mid, key) == 0) {
		float* item = get_item(mid);
		std::streampos pos = *reinterpret_cast<adress*>(item + 1);
		return pos;
	}
	else {
		throw Bad_Key("No such key.");
	}
}

void FileBase::delete_item(float key) {
	int last_block = ((adress)eof - sizeof(int)) / block_size;
	int mid_block = binary_search(&FileBase::check_block, 0, last_block, key);
	/*Считывание блока.*/
	if (mid_block != cur_block) {
		flush_block(cur_block);
		get_block(mid_block);
	}
	int mid = binary_search(&FileBase::check_item, 0, count - 1, key);
	/*Если элемент найден.*/
	if (check_item(mid,key) == 0) {
		float* item = get_item(mid);
		adress* adr = reinterpret_cast<adress*>(item + 1);

		/*Запись отрицательного размера.*/
		base.seekg((*adr) - sizeof(int));
		int size;
		base.read(reinterpret_cast<char*>(&size), sizeof(int));
		size *= -1;
		base.seekp((*adr) - sizeof(int));
		base.write(reinterpret_cast<char*>(&size), sizeof(int));
		base.seekp(0, std::ios::end);

		int i = 0;
		/*Сдвиг элементов влево.*/
		for (i = mid; i < count - 1; ++i) {
			float* cur_item = get_item(i);
			float* next_item = get_item(i + 1);
			adress* cur_adr = reinterpret_cast<adress*>(cur_item + 1);
			adress* next_adr = reinterpret_cast<adress*>(next_item + 1);
			*cur_item = *next_item;
			*cur_adr = *next_adr;
		}
		float* last_item = get_item(count-1);
		adress* last_adr = reinterpret_cast<adress*>(last_item + 1);
		*last_item = 0;
		*last_adr = 0;
	}
	else {
		throw Bad_Key("No such key.");
	}
}

void* FileBase::get_item(std::streampos pos) {
	adress p = (adress)pos;
	if (p < sizeof(int)) {
		//Неверный адрес.
		throw Bad_Pos("Incorrect position.");
	}
	base.seekg(0, std::ios::end);
	std::streampos position = base.tellg();
	if (pos > position) {
		//Неверный адрес.
		throw Bad_Pos("Incorrect position.");
	}

	base.seekg(p - sizeof(int));
	int ln;
	base.read(reinterpret_cast<char*>(&ln), sizeof(int));
	/*Если запись удалена.*/
	if (ln < 0) {
		base.seekp(0, std::ios::end);
		throw Bad_Item("Item is deleted");
	}
	/*Если выходит за пределы базы - критическая ошибка.*/
	if ((adress)pos + ln > (adress)position) {
		base.seekp(0, std::ios::end);
		flush_block(cur_block);
		if (base.is_open()) {
			base.close();
		}
		if (idx.is_open()) {
			idx.close();
		}
		if (name) {
			delete[] name;
			name = nullptr;
		}
		if (buf) {
			delete[] buf;
			buf = nullptr;
		}
		/*eof == -1*/
		eof = idx.tellg();
		cur_block = -1;
		std::cerr << "Data corruption." << std::endl;
		throw Critical_Error("Data corruption.");
	}
	char* ret = new char[ln];
	base.read(ret, ln);
	base.seekp(0, std::ios::end);
	return ret;
}

void FileBase::print_keys() {
	if (cur_block == -1)
		get_block(0);
	flush_block(cur_block);
	std::cout << "Элементы выводятся в формате [номер блока:номер элемента] ключ - файловый указатель." << std::endl;
	for (int i = 0; ; ++i) {
		std::streampos pos(sizeof(int) + block_size*i);
		if (pos >= eof) {
			break;
		}

		get_block(i);
		for (int j = 0; j < count; ++j) {
			float* item = get_item(j);
			std::cout << "[" << i << ":" << j << "] " << std::setprecision(7) << *item << " - " << *reinterpret_cast<adress*>(item + 1) << std::endl;
		}
	}
	std::cout << std::endl;
}

/*
*
*
private:
*
*
*/
/*Если ключ находится левее или этот блок пустой, возвращает -1.
Если ключ равен первому ключу этого блока, возвращает 0.
Если ключ находится правее, возвращает 1.*/
int FileBase::check_block(int block, float key) {
	float value;
	if (cur_block != block) {
		std::streampos pos = sizeof(int) + block_size*block;
		if (pos >= eof)
			return -1;
		idx.seekg(pos);
		idx.read(reinterpret_cast<char*>(&value), sizeof(float));
	}
	else {
		value = *reinterpret_cast<float*>(buf);
	}
	if (value == 0)
		return -1;
	if (key < value)
		return -1;
	else if (key == value)
		return 0;
	else return 1;
}
/*Если ключ находится левее или текущий ключ равен нулю, возвращает -1.
Если ключ равен первому ключу этого блока, возвращает 0.
Если ключ находится правее, возвращает 1.*/
int FileBase::check_item(int item, float key) {
	float value = *reinterpret_cast<float*>(buf + item_size*item);

	if (value == 0 && key != 0)
		return -1;

	if (key < value)
		return -1;
	else if (key == value)
		return 0;
	else return 1;
}

int FileBase::binary_search(int(FileBase::*p)(int item, float key), int left, int right, float key) {
	if (left == right)
		return left;
	/*Проверяем правую границу.*/
	int res = (this->*p)(right, key);
	if (res >= 0) {
		return right;
	}
	
	int mid = (left + right) / 2;
	for (;;) {
		if (right - left == 1) {
			if ((this->*p)(mid, key) < 0) {
				return left;
			}
			else return mid;
		}
		mid = (left + right) / 2;
		int check = (this->*p)(mid, key);
		if (check < 0) {
			right = mid;
		}
		else if (check == 0) {
			return mid;
		}
		else {
			left = mid;
		}
	}
}

void FileBase::add_back_and_push_from(int block) {
	flush_block(cur_block);
	int last_block = ((adress)eof - sizeof(int)) / block_size;
	get_block(last_block);
	flush_block(last_block);
	for (int i = last_block; i > block; --i) {
		get_block(i - 1);
		flush_block(i);
	}
}

float FileBase::rand_key() {
	float rn = 0;
	int num = rand() % 10;
	int exp = 1000000;
	for (int i = 0; i < 7; ++i, exp /= 10) {
		rn += num*exp;
		num = rand() % 10;
	}
	rn /= 1000000;
	int ex = 1 + rand() % 30;
	rn *= pow(10, ex);
	if (rn == 0)
		rn = rand_key();
	return rn;
}

void FileBase::get_block(int block) {
	if ((adress)eof == 0) {
		idx.seekg(0, std::ios::end);
		eof = idx.tellg();
	}
	std::streampos pos = sizeof(int) + block_size*block;
	if (pos < eof) {
		idx.seekg(pos);
		idx.read(buf, block_size);
	}
	else {
		for (int i = 0; i < block_size; ++i) {
			buf[i] = 0;
		}
	}
	cur_block = block;
}

void FileBase::flush_block(int block) {
	if (cur_block == -1)
		return;
	std::streampos pos(sizeof(int) + block_size*block);
	idx.seekp(pos);
	idx.write(buf, block_size);
	if (pos >= eof) {
		idx.seekg(0, std::ios::end);
		eof = idx.tellg();
	}
}

float* FileBase::get_item(int item) {
	return reinterpret_cast<float*>(buf + item_size*item);
}

#undef _CRT_SECURE_NO_WARNINGS