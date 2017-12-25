#define _CRT_SECURE_NO_WARNINGS
#include "FileBase.h"

bool FileBase::is_randed = false;

FileBase::Bad_Item::Bad_Item() : std::exception() {}
FileBase::Bad_Item::Bad_Item(char const* const _Message) : std::exception(_Message) {}
FileBase::Bad_Open::Bad_Open() :std::exception() {}
FileBase::Bad_Open::Bad_Open(char const* const _Message): std::exception(_Message) {}

FileBase::FileBase() :/*max_key(0), beg(0), tail(0),*/ item_size(0), block_size(0), cur_block(-1), buf(nullptr)/*, is_locked(true)*/ {
	//is_randed = is_randed ? true : false;
}

FileBase::FileBase(const char* name, int idx_count/*, bool locked*/):FileBase() {
	open(name, idx_count/*, locked*/);
}

FileBase::~FileBase() {
	flush_block(cur_block);
	/*if (!is_locked)
		compress();*/
	if (base)
		base.close();
	if (idx)
		idx.close();
	delete[] name;
	delete[] buf;
}

void FileBase::open(const char* name, int idx_count/*, bool locked*/) {
	if (!is_randed) {
		srand(time(NULL));
		is_randed = true;
	}
	/*is_locked = locked;*/
	count = idx_count;
	int mode = std::ios::in | std::ios::out | std::ios::binary | std::ios::ate;

	int ln = 4 + sizeof(name) + 1;
	this->name = new char[ln-4];
	strcpy(this->name, name);
	char* temp = new char[ln];
	memcpy(temp, "idx_", 5);
	strcat(temp, name);

	try {
		base.open(name, mode);
		idx.open(temp, mode);
		if (!base || !idx)
			throw Bad_Open("Couldn't open the base.");
	}
	catch (std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		throw;
	}
	/*Получение данных из существующей базы.*/
	idx.seekg(0, std::ios::end);
	if (idx.tellg() != std::streampos(0)) {

		idx.seekg(0);
		idx.read((char*)&count, sizeof(int));
		/*idx.read((char*)beg, sizeof(adress));
		idx.read((char*)tail, sizeof(adress));*/

		item_size = sizeof(float) + sizeof(adress);
		block_size = item_size*count;

		/*idx.seekg(-block_size, std::ios::end);*/

		buf = new char[block_size];
		/*idx.read(buf, block_size);

		char* it = buf + block_size - item_size;
		while (*((float*)it) == 0) {
			it -= item_size;
		}
		max_key = *((float*)it);
		cur_block = (max_key-1) / count;*/
	}
	else { 
		item_size = sizeof(float) + sizeof(adress);
		block_size = item_size*count;
		buf = new char[block_size];
	}
	/*for (int i = 0; i < block_size; ++i) {
		buf[i] = 0;
	}*/
}

float FileBase::insert_item(void* data, int size) {

	regenerate:
	/*TODO: Сгенерировать ключ.*/
	float key = rand_key();
	/*TODO: Найти место для вставки.*/
	int block_pos = 0;
	std::streampos left(sizeof(int) + block_pos*block_size);
	idx.seekg(0, std::ios::end);
	if ((adress)eof == 0) {
		eof = idx.tellg();
	}
	if ((adress)eof == 0) {
		idx.write((char*)&count, sizeof(count));
		idx.seekg(0, std::ios::end);
		eof = idx.tellg();
	}
	//else idx.seekg(-block_size, std::ios::end);
	std::streampos right = idx.tellg();
	//right

	int mid_block;
	if (left == right) {
		mid_block = 0;//GOTO: Вставка в блок mid_block.
		goto insert;
	}

	int left_block = 0;
	int right_block = ((adress)right - sizeof(int)) / block_size;
	//int (FileBase::*p)(int,float) = &FileBase::check_block;
	mid_block = binary_search(&FileBase::check_block, left_block, right_block, key);
	if (mid_block == -1) {
		goto regenerate;
	}
	//for (;;) {
	//	if (right_block - left_block == -1) {
	//		goto insert;//GOTO: Вставка в блок mid_block.
	//	}
	//	mid_block = (left_block + right_block) / 2;
	//	int check = check_block(mid_block, key);
	//	if (check < 0) {
	//		right_block = mid_block;
	//	}
	//	else if (check == 0) {
	//		goto insert;//GOTO: Вставка в блок mid_block.
	//	}
	//	else {
	//		left_block = mid_block;
	//	}
	//}

	insert:
	/*TODO: Вставить.*/
	if (cur_block != mid_block) {
		flush_block(cur_block);
		/*if (((adress)eof - sizeof(int)) / block_size <= mid_block) {
			for (int i = 0; i < block_size; ++i) {
				buf[i] = 0;
			}
		}
		else*/ get_block(mid_block);
		cur_block = mid_block;
	}
	float* item = get_item(count-1);
	char* temp = nullptr;
	bool is_overflow = false;
	if (*item != 0) {
		is_overflow = true;//Переполнение.
		temp = new char[item_size];
		memcpy(temp, item, item_size);
	}
	{
		item = get_item(count / 2);
		int left, right;
		left = 0;
		right = count - 1;
		/*if (*item != 0) {
			
		}
		else {
			left = 0;
			right = count / 2;
		}*/
		int mid = binary_search(&FileBase::check_item, left, right, key);
		if (mid == -1) {
			goto regenerate;
		}

		int place_to_insert = 0;
		int check = check_item(mid, 0);
		if (check == 0) {
			place_to_insert = mid;
		}
		else {
			for (;;) {
				int check = check_item(mid, key);
				if (check > 0) {
					//place_to_insert = mid + 1;
					++mid;
				}
				else {
					place_to_insert = mid;
					//int size = (count - place_to_insert - 1)*item_size;
					for (int i = block_size - 1; i >= (place_to_insert + 1)*item_size; --i) {
						buf[i] = buf[i - item_size];
					}
					break;
					/*char* temp = new char[size];
					memcpy(temp, buf + place_to_insert, size);
					memcpy(buf + place_to_insert + item_size, temp, size);*/
				}
			}
		}
		
		float* item = get_item(place_to_insert);
		*item = key;
		item++;

		/*Запись в базу данных и сохранение индекса.*/
		base.write((char*)&size, sizeof(int));
		*((adress*)item) = base.tellp();
		base.write((char*)data, size);

		//int mid;
		//for (;;) {
		//	if (right - left == -1) {
		//		;//GOTO: Вставка после элемента mid.
		//	}
		//	mid = (left + right) / 2;
		//	int check = check_item(mid, key);
		//	if (check < 0) {
		//		right = mid;
		//	}
		//	else if (check == 0) {
		//		;//ERROR: Ключи совпадают.
		//	}
		//	else {
		//		left = mid;
		//	}
		//}
	}
	/*TODO: Перенести, если переполнение.*/
	if (is_overflow) {
		
		//int cnt = ceil((float)count / 2);
		////print_keys();
		//int l_cnt = count - cnt;
		//char* moved = new char[item_size*cnt];
		//memcpy(moved, buf + item_size*(l_cnt + 1), item_size*(cnt - 1));
		//memcpy(moved + item_size*(cnt - 1), temp, item_size);
		//for (int i = item_size*(l_cnt + 1); i < block_size; ++i) {
		//	buf[i] = 0;
		//}
		//while (cnt > 0) {
		//	flush_block(cur_block);
		//	get_block(++cur_block);
		//	int pos = 0;
		//	for (int i = block_size - item_size; i >= 0; i -= item_size) {
		//		if (buf[i] != 0) {
		//			pos = i / item_size;
		//			break;
		//		}
		//	}
		//	int offset = count - pos - 1;
		//	int tmp_cnt = cnt - offset;
		//	char* tmp = nullptr;
		//	if (tmp_cnt > 0) {
		//		tmp = new char[item_size*tmp_cnt];
		//		memcpy(tmp, buf + item_size*(pos - (tmp_cnt - 1)), item_size*(tmp_cnt));

		//		for (int i = 0; i < item_size*(pos - (tmp_cnt - 1)); ++i) {
		//			buf[i + item_size*(pos - (tmp_cnt - 1))] = buf[i];
		//		}
		//	}
		//	else {
		//		for (int i = block_size - 1; i >= item_size*cnt; --i) {
		//			buf[i] = buf[i - item_size*cnt];
		//		}
		//		/*for (int i = 0; i < item_size*cnt; ++i) {
		//			buf[i + item_size*cnt] = buf[i];
		//		}		*/		
		//	}

		//	memcpy(buf, moved, item_size*cnt);
		//	delete[] moved;
		//	moved = tmp;
		//	cnt = tmp_cnt;
		//	//print_keys();
		//}
		//if (moved)
		//	delete[] moved;
		//if (temp)
		//	delete[] temp;

		int cnt = ceil((float)count / 2);
		//print_keys();
		int l_cnt = count - cnt;
		char* moved = new char[item_size*cnt];
		memcpy(moved, buf + item_size*(l_cnt + 1), item_size*(cnt - 1));
		memcpy(moved + item_size*(cnt - 1), temp, item_size);
		for (int i = item_size*(l_cnt + 1); i < block_size; ++i) {
			buf[i] = 0;
		}
		int block_pos = cur_block;
		add_back_and_push_from(cur_block + 1);
		for (int i = 0; i < block_size; ++i) {
			buf[i] = 0;
		}
		memcpy(buf, moved, item_size*cnt);
		delete[] moved;
		delete[] temp;

		flush_block(block_pos + 1);
		cur_block = block_pos + 1;

	}
	//print_keys();
	return key;
	
	/*Нулевой ключ зарезервирован для пустых записей.*/
	/*max_key++;

	if (max_key == 1)
	{
		idx.write((char*)&count, sizeof(int));
		idx.write((char*)&beg, sizeof(adress));
		idx.write((char*)&tail, sizeof(adress));
	}*/

	///*В каком блоке находится элемент.*/
	//int block_pos = (max_key - 1) / count;
	///*Позиция блока.*/
	//std::streampos p(sizeof(int) + 2 * sizeof(adress) + block_pos*block_size);
	///*Вставка на место первой удаленной записи, если они незакреплённые.*/
	//if (!is_locked) {
	//	float result = insert_on_deleted(data, size);
	//	if (result != 0)
	//		return result;
	//}

	//if (block_pos != cur_block) {
	//	if (cur_block != -1) {
	//		flush_block(cur_block);
	//	}
	//	/*Для выяснения, будет ли это новый блок.*/
	//	idx.seekg(0, std::ios::end);
	//	std::streampos pp = idx.tellg();
	//	/*Если этот блок будет новым.*/
	//	if (p == pp) {
	//		int i = 0;
	//		while (i < block_size)
	//			buf[i++] = 0;
	//	}
	//	else {
	//		get_block(p);
	//	}
	//	cur_block = block_pos;
	//}

	///*Куда будет записываться новые ключ и адрес.*/
	//float* item = get_item(((int)max_key - 1) % count);
	//*item = max_key;
	//item++;

	///*Запись в базу данных и сохранение индекса.*/
	//base.write((char*)&size, sizeof(int));
	//*((adress*)item) = base.tellp();
	//base.write((char*)data, size);
	//
	//return max_key;
}

float FileBase::insert_item(InterBase* p) {
	void* data = p->get_data();
	float result = insert_item(data, p->size_of_data());
	delete[] data;
	return result;
}
//
//std::streampos FileBase::search_item(float key) {
//	key = (int)key;
//	int block_pos = (key - 1) / count;
//	if (cur_block != block_pos) {
//		/*Позиция блока.*/
//		std::streampos p(sizeof(int) + 2 * sizeof(adress) + block_pos*block_size);
//		idx.seekg(0, std::ios::end);
//		std::streampos pp = idx.tellg();
//		if (p >= pp) {
//			//Такого элемента нет.
//			throw Bad_Item("No such item.");
//		}
//		flush_block(cur_block);
//		get_block(p);
//	}
//
//	float* item = get_item(((int)key - 1) % count);
//	if (*item != key) {
//		//Такого элемента нет.
//		if (int(*item) == 0)
//			throw Bad_Item("No such item.");
//		else throw Bad_Item("Item is deleted");
//	}
//	else {
//		item++;
//
//		base.seekg((*(adress*)item) - sizeof(int));
//		int size;
//		base.read((char*)&size, sizeof(int));
//		base.seekp(0, std::ios::end);
//		if (size <= 0) {
//			throw Bad_Item("Item is deleted");
//		}
//		return std::streampos(*((adress*)item));
//	}	
//}
//
//void FileBase::delete_item(float key) {
//	key = (int)key;
//	int block_pos = (key - 1) / count;
//	/*Позиция блока.*/
//	std::streampos p(sizeof(int) + 2 * sizeof(adress) + block_pos*block_size);
//
//	if (cur_block != block_pos) {		
//		idx.seekg(0, std::ios::end);
//		std::streampos pp = idx.tellg();
//		if (p >= pp) {
//			//Такого элемента нет.
//			throw Bad_Item("No such item.");
//		}
//		flush_block(cur_block);
//		get_block(p);
//	}
//
//	float* item = get_item(((int)key - 1) % count);
//	if (*item != key) {
//		//Такого элемента нет.
//		if (int(*item) == 0)
//			throw Bad_Item("No such item.");
//		else throw Bad_Item("Item is deleted");
//	}
//	else {
//		/*Вставка в список удалённых элементов.*/
//		if (tail != 0) {
//			std::streampos tail_pos(tail + sizeof(float));
//			idx.seekp(tail_pos);
//			tail = (adress)((char*)item - buf) + p;
//			idx.write((char*)&tail, sizeof(adress));
//			idx.seekp(sizeof(int) + sizeof(adress), std::ios::beg);
//			idx.write((char*)&tail, sizeof(adress));
//		}
//		else {
//			beg = tail = (adress)((char*)item - buf) + p;
//			idx.seekp(sizeof(int), std::ios::beg);
//			idx.write((char*)&beg, sizeof(adress));
//			idx.write((char*)&tail, sizeof(adress));
//		}
//		(*item) += 0.1;
//
//		++item;
//		/*Запись отрицательного размера в базу данных.*/
//		base.seekg((*(adress*)item) - sizeof(int));
//		int size;
//		base.read((char*)&size, sizeof(int));
//		size *= -1;
//		base.seekp((*(adress*)item) - sizeof(int));
//		base.write((char*)&size, sizeof(int));
//		base.seekp(0, std::ios::end);
//	}
//
//}
//
//void* FileBase::get_item(std::streampos pos) {
//	adress p = (adress)pos;
//	if (p < sizeof(int)) {
//		//Неверный адрес.
//		return nullptr;
//	}
//	base.seekg(0, std::ios::end);
//	std::streampos position = base.tellg();
//	if (pos > position) {
//		//Неверный адрес.
//		return nullptr;
//	}
//
//	base.seekg(p - sizeof(int));
//	int ln;
//	base.read((char*)&ln, sizeof(int));
//	if (ln < 0) {
//		//Запись удалена.
//		return nullptr;
//	}
//	char* ret = new char[ln];
//	base.read((char*)ret, ln);
//	return ret;
//}
//
//void FileBase::compress() {	
//	namespace fs = std::experimental::filesystem::v1;
//
//	fs::path path = fs::current_path();
//	char* temp = new char[strlen(name) + 6];
//	memcpy(temp, "tmp_", 5);
//	int i, j;
//	strcat(temp, name);
//	/*Переименования файла во временный.*/
//	base.close();
//	fs::rename(path / name, path / temp);
//	std::ifstream in(temp, std::ios::binary);
//	std::ofstream out(name, std::ios::binary);
//
//	idx.seekg(0, std::ios::end);
//	adress size = idx.tellg();
//	char* str = new char[128];
//	/*Выборочное копирование базы данных.*/
//	for (int i = 0;;++i) {
//		if (sizeof(int) + 2 * sizeof(adress) + i*block_size >= size)
//			break;
//
//		get_block(i);
//		
//		for (int j = 0; j < count; ++j) {
//			char* p = buf + j*block_size;
//			int integer = *((float*)p);
//			if (*((float*)p) == integer) {
//				in.seekg(*((adress*)(p + sizeof(float))) - sizeof(int));
//				int ln;
//				in.read((char*)&ln, sizeof(int));
//				out.write((char*)&ln, sizeof(int));
//				*(adress*)(p + sizeof(float)) = out.tellp();
//				while (ln > 0) {
//					int r;
//					if (ln / 128 >= 1)
//						r = 128;
//					else r = ln % 128;
//					ln -= r;
//					in.read(str, r);
//					out.write(str, r);
//				}
//				
//			}
//		}
//
//		flush_block(i);
//	}
//	in.close();
//	/*Удаление старого файла.*/
//	fs::remove(path / temp);
//	int mode = std::ios::in | std::ios::out | std::ios::binary | std::ios::ate;
//	base.open(name, mode);
//}

/*
*
*
private:
*
*
*/

void FileBase::print_keys() {
	if (cur_block == -1)
		cur_block = 0;
	flush_block(cur_block);
	for (int i = 0; ; ++i) {
		std::streampos pos(sizeof(int) + block_size*i);
		if (pos >= eof) {
			break;
		}

		get_block(i);
		for (int j = 0; j < count; ++j) {
			float* item = get_item(j);
			std::cout << i << "." << j << ". " << *item << std::endl;
		}
	}
	std::cout << std::endl;
}

int FileBase::check_block(int block, float key) {
	float value;
	if (cur_block != block) {
		std::streampos pos = get_pos(block);
		if (pos >= eof)
			return -1;
		idx.seekg(pos);
		idx.read((char*)&value, sizeof(float));
	}
	else {
		value = *reinterpret_cast<float*>(buf);
	}
	if (key < value)
		return -1;
	else if (key == value)
		return 0;
	else return 1;
}

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
	int res = (this->*p)(right, key);
	if (res > 0) {
		return right;
	}
	else if (res == 0) {
		return -1;
	}
	int mid = (left + right) / 2;
	for (;;) {
		if (right - left == 1) {
			if ((this->*p)(mid, key) < 0) {
				return left;
			}
			else return mid;//GOTO: Вставка после элемента mid.
		}
		mid = (left + right) / 2;
		int check = (this->*p)(mid, key);
		if (check < 0) {
			right = mid;
		}
		else if (check == 0) {
			return -1;//ERROR: Ключи совпадают.
		}
		else {
			left = mid;
		}
	}
}

std::streampos FileBase::get_pos(int block) {
	return std::streampos(sizeof(int) + block_size*block);
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

//float FileBase::insert_on_deleted(void* data, int size) {
//	int block_pos = (max_key - 1) / count;
//	/*Позиция блока.*/
//	std::streampos p(sizeof(int) + 2 * sizeof(adress) + block_pos*block_size);
//
//	/*Если в списке удалённых элементов есть хотя бы один.*/
//	if (beg != 0) {
//		adress cur = beg;
//		if (beg == tail) {
//			beg = tail = 0;
//			idx.seekp(sizeof(int), std::ios::beg);
//			idx.write((char*)&beg, sizeof(adress));
//			idx.write((char*)&tail, sizeof(adress));
//		}
//		else {
//			std::streampos cur_pos(cur + sizeof(float));
//			idx.seekg(cur_pos);
//			idx.read((char*)&beg, sizeof(adress));
//			idx.seekp(sizeof(int), std::ios::beg);
//			idx.write((char*)&beg, sizeof(adress));
//		}
//
//		cur -= sizeof(int) + 2 * sizeof(adress);
//		int cur_pos = cur / item_size;
//		int block_pos = cur_pos / count;
//		std::streampos p(sizeof(int) + 2 * sizeof(adress) + block_pos*block_size);
//		if (block_pos != cur_block) {
//			flush_block(cur_block);
//			get_block(p);
//		}
//
//		/*Куда будут записываться новые ключ и адрес.*/
//		float* item = get_item(cur_pos % count);
//		*item -= 0.1;
//		float result = *item;
//		item++;
//
//		/*Запись в базу данных и сохранение индекса.*/
//		base.write((char*)&size, sizeof(int));
//		*((adress*)item) = base.tellp();
//		base.write((char*)data, size);
//
//		return result;
//	}
//	/*Если до этого ничего не удалялось.*/
//	else {
//		return 0;
//	}
//}

float FileBase::rand_key() {
	float rn = 0;
	int num = rand() % 10;
	for (int i = 0; i < 7; ++i) {
		rn += num*pow(10, 6 - i);
		num = rand() % 10;
	}
	rn /= 1000000;
	int ex = 1 + rand() % 30;
	rn *= pow(10, ex);
	//std::cout << rn << std::endl;
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

void FileBase::get_block(std::streampos pos) {
	idx.seekg(pos);
	idx.read(buf, block_size);
	cur_block = (((int) pos) - sizeof(int) - 2* sizeof(adress))/block_size;
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
	return (float*)(buf + item_size*item);
}

#undef _CRT_SECURE_NO_WARNINGS