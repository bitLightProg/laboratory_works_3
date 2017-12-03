#include <type_traits>

template<typename TL>
class Template {
	TL data;
public:
	int size_of_data() {
		return sizeof(data);
	}
};

template<typename TL>
class Template<TL*> {
	TL data;
public:
	int size_of_data() {
		Template<TL> a;
		return a.size_of_data();
	}
};