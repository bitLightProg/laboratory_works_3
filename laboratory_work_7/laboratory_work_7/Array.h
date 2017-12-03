#pragma once

template <class T, int lenght>
class Array {
public:
	Array();
	Array(T a[lenght]);
	~Array();
	void print(std::ostream &os = std::cout);
	void sort(int l = 0, int r = lenght - 1);
	T* find(const T& key);
	T& operator[](int i);
private:
	T arr[lenght];
	int partion(int l, int r);
};

template<class T, int lenght>
inline Array<T, lenght>::Array() {
	for (int i = 0; i < lenght; ++i) {
		arr[i] = T();
	}
}

template<class T, int lenght>
inline Array<T, lenght>::Array(T a[lenght]) {
	for (int i = 0; i < lenght; ++i) {
		arr[i] = a[i];
	}
}

template<class T, int lenght>
inline Array<T, lenght>::~Array() {
}

template<class T, int lenght>
inline void Array<T, lenght>::print(std::ostream& os) {
	for (int i = 0; i < lenght; ++i) {
		os << arr[i] << " ";
	}
	os << std::endl;
}

template<class T, int lenght>
void Array<T, lenght>::sort(int l, int r) {
	if (l < 0) l = 0;
	if (r >= lenght) r = lenght - 1;

	if (r <= l) return;
	int i = partion(l, r);
	sort(l, i - 1);
	sort(i + 1, r);
}

template<class T, int lenght>
inline T* Array<T, lenght>::find(const T& key) {
	int l = 0, r = lenght - 1;
	while (l < r) {
		int i = (l + r) / 2;
		if (arr[i] < key)
			l = i + 1;
		else if (arr[i] > key)
			r = i - 1;
		else return arr + i;
	}
	if (arr[l] == key)
		return arr+l;
	else return nullptr;
}

template<class T, int lenght>
inline T& Array<T, lenght>::operator[](int i) {
	if (i < lenght) {
		return arr[i];
	}
}

template<class T, int lenght>
int Array<T, lenght>::partion(int l, int r) {
	int i = l - 1, j = r;
	for (;;) {
		while (arr[++i] < arr[r]);
		while (j >= 0 && arr[r] < arr[--j]) {
			if (j == 1 && i != 0)
				break;
		}
		if (i >= j) break;
		std::swap(arr[i], arr[j]);
	}
	std::swap(arr[i], arr[r]);
	return i;
}
