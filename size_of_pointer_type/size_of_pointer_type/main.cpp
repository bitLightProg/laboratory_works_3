#include <iostream>

#include "Template.h"

int main() {
	Template<char******> a;
	std::cout << a.size_of_data() << std::endl;
	std::cout << sizeof(char******) << std::endl;
	system("pause");
	return 0;
}