#define _CRT_SECURE_NO_WARNINGS
#include "FileBase.h"

FileBase::Bad_Item::Bad_Item() : std::exception() {}
FileBase::Bad_Item::Bad_Item(char const* const _Message) : std::exception(_Message) {}
FileBase::Bad_Open::Bad_Open() :std::exception() {}
FileBase::Bad_Open::Bad_Open(char const* const _Message): std::exception(_Message) {}

FileBase::FileBase() :max_key(0), beg(0), tail(0), item_size(0), block_size(0), cur_block(-1), buf(nullptr), is_locked(true) {

}

FileBase::FileBase(const char* name, int idx_count, bool locked):FileBase() {
	open(name, idx_count, locked);
}

FileBase::~FileBase() {
	flush_block(cur_block);
	if (!is_locked)
		compress();
	if (base)
		base.close();
	if (idx)
		idx.close();
}

void FileBase::open(const char* name, int idx_count, bool locked) {
	is_locked = locked;
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
			throw Bad_Open("Couldn't open the base.");//std::exception("Couldn't open the base.");
	}
	catch (std::exception ex) {
		std::cerr << ex.what() << std::endl;
		throw;
	}

	idx.seekg(0, std::ios::end);
	if (idx.tellg() != std::streampos(0)) {

		idx.seekg(0);
		idx.read((char*)&count, sizeof(int));
		idx.read((char*)beg, sizeof(adress));
		idx.read((char*)tail, sizeof(adress));

		item_size = sizeof(float) + sizeof(adress);
		block_size = item_size*count;

		idx.seekg(-block_size, std::ios::end);

		buf = new char[block_size];
		idx.read(buf, block_size);

		char* it = buf + block_size - item_size;
		while (*((float*)it) == 0) {
			it -= item_size;
		}
		max_key = *((float*)it);
		cur_block = (max_key-1) / count;
		//delete buf;

	}
	else { 
		//max_key = 0;
		item_size = sizeof(float) + sizeof(adress);
		block_size = item_size*count;
		buf = new char[block_size];
		//cur_block = -1;
	}
}

float FileBase::insert_item(void* data, int size) {
	/*Нулевой ключ зарезервирован для пустых записей.*/
	max_key++;

	if (max_key == 1)
	{
		idx.write((char*)&count, sizeof(int));
		idx.write((char*)&beg, sizeof(adress));
		idx.write((char*)&tail, sizeof(adress));
		//idx.flush();
	}

	/*Размер элемента.*/
	//int item_size = sizeof(float) + sizeof(adress);
	//int block_size = item_size*count;
	/*В каком блоке находится элемент.*/
	int block_pos = (max_key - 1) / count;
	/*Позиция блока.*/
	std::streampos p(sizeof(int) + 2 * sizeof(adress) + block_pos*block_size);
	if (!is_locked) {
		float result = insert_on_deleted(data, size);
		if (result != 0)
			return result;
	}

	if (block_pos != cur_block) {
		if (cur_block != -1) {
			flush_block(cur_block);
		}
		/*Для выяснения, будет ли это новый блок.*/
		idx.seekg(0, std::ios::end);
		std::streampos pp = idx.tellg();
		/*Если этот блок будет новым.*/
		if (p == pp) {
			int i = 0;
			while (i < block_size)
				buf[i++] = 0;
		}
		else {
			get_block(p);
		}
		cur_block = block_pos;
	}
	/*Куда будет записываться новые ключ и адрес.*/
	float* item = get_item(((int)max_key - 1) % count);//(float*)(buf + ((((int)max_key - 1) % count)*item_size));
	*item = max_key;
	item++;

	/*Запись в базу данных и сохранение индекса.*/
	base.write((char*)&size, sizeof(int));
	*((adress*)item) = base.tellp();
	base.write((char*)data, size);
	/*Запись блока.*/
	//idx.seekp(p);
	//idx.write(buf, block_size);

	//delete buf;
	
	return max_key;
}

float FileBase::insert_item(InterBase* p) {
	void* data = p->get_data();
	float result = insert_item(data, p->size_of_data());
	delete[] data;
	return result;
}

std::streampos FileBase::search_item(float key) {
	key = (int)key;
	int block_pos = (key - 1) / count;
	if (cur_block != block_pos) {
		/*Позиция блока.*/
		std::streampos p(sizeof(int) + 2 * sizeof(adress) + block_pos*block_size);
		idx.seekg(0, std::ios::end);
		std::streampos pp = idx.tellg();
		if (p >= pp) {
			//Такого элемента нет.
			throw std::exception("No such item.");
			//return std::streampos(-1);
		}

		get_block(p);
	}

	float* item = get_item(((int)key - 1) % count);//(float*)(buf + ((((int)key - 1) % count)*item_size));
	if (*item != key) {
		//Такого элемента нет.
		if (int(*item) == 0)
			throw std::exception("No such item.");
		else throw std::exception("Item is deleted");
		//return std::streampos(-1);
	}
	else {
		item++;

		base.seekg((*(adress*)item) - sizeof(int));
		int size;
		base.read((char*)&size, sizeof(int));
		base.seekp(0, std::ios::end);
		if (size <= 0) {
			throw std::exception("Item is deleted");
		}
		return std::streampos(*((adress*)item));
	}	
}

void FileBase::delete_item(float key) {
	key = (int)key;
	int block_pos = (key - 1) / count;
	/*Позиция блока.*/
	std::streampos p(sizeof(int) + 2 * sizeof(adress) + block_pos*block_size);

	if (cur_block != block_pos) {		
		idx.seekg(0, std::ios::end);
		std::streampos pp = idx.tellg();
		if (p >= pp) {
			//Такого элемента нет.
			throw Bad_Item("No such item.");
		}
		flush_block(cur_block);
		get_block(p);
	}

	float* item = get_item(((int)key - 1) % count);//(float*)(buf + ((((int)key - 1) % count)*item_size));
	if (*item != key) {
		//Такого элемента нет.
		throw Bad_Item("No such item.");
	}
	else {
		if (tail != 0) {
			std::streampos tail_pos(tail + sizeof(float));
			idx.seekp(tail_pos);
			tail = (adress)((char*)item - buf) + p;
			idx.write((char*)&tail, sizeof(adress));
			idx.seekp(sizeof(int) + sizeof(adress), std::ios::beg);
			idx.write((char*)&tail, sizeof(adress));
		}
		else {
			beg = tail = (adress)((char*)item - buf) + p;
			idx.seekp(sizeof(int), std::ios::beg);
			idx.write((char*)&beg, sizeof(adress));
			idx.write((char*)&tail, sizeof(adress));
		}
		(*item) += 0.1;

		++item;
		base.seekg((*(adress*)item) - sizeof(int));
		int size;
		base.read((char*)&size, sizeof(int));
		size *= -1;
		base.seekp((*(adress*)item) - sizeof(int));
		base.write((char*)&size, sizeof(int));
		base.seekp(0, std::ios::end);
	}

}

void FileBase::get_item(void* &dest, std::streampos pos) {
	adress p = (adress)pos;
	if (dest)
		delete[] dest;
	if (p < sizeof(int)) {
		//Неверный адрес.
		return;
	}
	base.seekg(p - sizeof(int));
	int ln;
	base.read((char*)&ln, sizeof(int));
	dest = new char[ln];
	base.read((char*)dest, ln);
}

void FileBase::compress() {
	
	//const char* name = ((path.filename()).string()).c_str();
	namespace fs = std::experimental::filesystem::v1;
	fs::path path = fs::current_path();
	char* temp = new char[strlen(name) + 6];
	memcpy(temp, "tmp_", 5);
	int i, j;
	strcat(temp, name);
	//strcat(temp, name);
	//path.replace_filename(temp);
	//std::experimental::filesystem::v1::path new_path;
	//new_path.assign(temp);
	//fs::create_directory(path / temp);
	base.close();
	fs::rename(path / name, path / temp);
	std::ifstream in(temp, std::ios::binary);
	std::ofstream out(name, std::ios::binary);

	idx.seekg(0, std::ios::end);
	adress size = idx.tellg();
	char* str = new char[128];

	for (int i = 0;;++i) {
		if (sizeof(int) + 2 * sizeof(adress) + i*block_size >= size)
			break;

		get_block(i);
		
		for (int j = 0; j < count; ++j) {
			char* p = buf + j*block_size;
			int integer = *((float*)p);
			if (*((float*)p) == integer) {
				in.seekg(*((adress*)(p + sizeof(float))) - sizeof(int));
				//std::stringstream str;
				int ln;
				in.read((char*)&ln, sizeof(int));
				out.write((char*)&ln, sizeof(int));
				*(adress*)(p + sizeof(float)) = out.tellp();
				while (ln > 0) {
					int r;
					if (ln / 128 >= 1)
						r = 128;
					else r = ln % 128;
					ln -= r;
					in.read(str, r);
					out.write(str, r);
				}
				
			}
		}

		flush_block(i);

	}
	in.close();
	fs::remove(path / temp);
	int mode = std::ios::in | std::ios::out | std::ios::binary | std::ios::ate;
	base.open(name, mode);
//	fs::rename(path / name, path / temp);
}

float FileBase::insert_on_deleted(void* data, int size) {
	int block_pos = (max_key - 1) / count;
	/*Позиция блока.*/
	std::streampos p(sizeof(int) + 2 * sizeof(adress) + block_pos*block_size);

	if (block_pos != cur_block) {
		if (beg != 0) {
			adress cur = beg;
			if (beg == tail) {
				beg = tail = 0;
				idx.seekp(sizeof(int), std::ios::beg);
				idx.write((char*)&beg, sizeof(adress));
				idx.write((char*)&tail, sizeof(adress));
			}
			else {
				std::streampos cur_pos(cur + sizeof(float));
				idx.seekg(cur_pos);
				idx.read((char*)&beg, sizeof(adress));
				idx.seekp(sizeof(int), std::ios::beg);
				idx.write((char*)&beg, sizeof(adress));
			}

			cur -= sizeof(int) + 2 * sizeof(adress);
			int cur_pos = cur / item_size;
			int block_pos = cur_pos / count;
			std::streampos p(sizeof(int) + 2 * sizeof(adress) + block_pos*block_size);
			if (block_pos != cur_block) {
				flush_block(cur_block);
				get_block(p);
			}

			/*Куда будет записываться новые ключ и адрес.*/
			float* item = get_item(cur_pos % count);//(float*)(buf + ((((int)cur_pos) % count)*item_size));
			*item -= 0.1;
			float result = *item;
			item++;

			/*Запись в базу данных и сохранение индекса.*/
			base.write((char*)&size, sizeof(int));
			*((adress*)item) = base.tellp();
			base.write((char*)data, size);
			/*Запись блока.*/
			//idx.seekp(p);
			//idx.write(buf, block_size);

			//delete buf;

			return result;

		}
		else {
			return 0;
		}

		//idx.seekp(p, std::ios::beg);

		//char* buf = new char[block_size];
		if (cur_block != -1) {
			flush_block(cur_block);
		}
		/*Для выяснения, будет ли это новый блок.*/
		idx.seekg(0, std::ios::end);
		std::streampos pp = idx.tellg();
		/*Если этот блок будет новым.*/
		if (p == pp) {
			int i = 0;
			while (i < block_size)
				buf[i++] = 0;
		}
		else {
			get_block(p);
		}
		cur_block = block_pos;
	}
	else {
		return 0;
	}
}

void FileBase::get_block(int block) {
	idx.seekg(sizeof(int) + 2 * sizeof(adress) + block_size*block);
	idx.read(buf, block_size);
	cur_block = block;
}

void FileBase::get_block(std::streampos pos) {
	idx.seekg(pos);
	idx.read(buf, block_size);
	cur_block = (((int) pos) - sizeof(int) - 2* sizeof(adress))/block_size;
}

void FileBase::flush_block(int block) {
	std::streampos pos(sizeof(int) + 2 * sizeof(adress) + block_size*block);
	idx.seekp(pos);
	idx.write(buf, block_size);
}

//void FileBase::flush_block(std::streampos pos) {
//	idx.seekp(pos);
//	idx.write(buf, block_size);
//	//idx.flush();
//}

float* FileBase::get_item(int item) {
	return (float*)(buf + item_size*item);
}

#undef _CRT_SECURE_NO_WARNINGS