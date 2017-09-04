#include <string>
#include <iostream>
#include <Windows.h>

class EviTree {
public:
	EviTree(): parent(nullptr), children(nullptr), hypo(nullptr), concl(nullptr) {}
	~EviTree() {
		if (hypo) {
			delete[] hypo;
		}

		if (concl) {
			delete[] concl;
		}
	}

	void SetHypo(char *& h)
	{
		hypo = new char[strlen(h) + 1];
		int i = 0;
		while (h[i] != '\0') {
			hypo[i] = h[i++];
		}
		hypo[i] = '\0';
	}

	void SetConcl(char *& c)
	{
		concl = new char[strlen(c) + 1];
		int i = 0;
		while (c[i] != '\0') {
			concl[i] = c[i++];
		}
		concl[i] = '\0';
	}

	const char* GetHypo()
	{
		return hypo;
	}

	const char* GetConcl()
	{
		return concl;
	}

private:
	EviTree *parent;
	EviTree *children;
	char *hypo;
	char *concl;
};

/*Получает ввод.
Принимает char* на гипотезу и char* на заключение.
Память переданных указателей следует освободить с помощью delete[].*/
void input(char* &hypothesis, char* &conclusion) {
	std::cout << "Введите высказывание в виде: not a and not (b or c) -> d, a |- a ;"
		<< "a |- (составные знаки должны быть нераздельны)." << std::endl;

	char *temp = new char[256];
	std::cin.getline(temp, 255);
	temp[256] = '\0';

	if (hypothesis) {
		delete[] hypothesis;
		hypothesis = nullptr;
	}

	if (conclusion) {
		delete[] conclusion;
		conclusion = nullptr;
	}

	int i, ln = strlen(temp), pos = -1;
	for (i = 0; i < ln; ++i) {
		if (pos == -1 && temp[i] == '|' && temp[i + 1] == '-') {
			pos = i;
			++i;
			continue;
		}

		if (pos != -1 && temp[i] != ' ')
			break;
	}

	if (pos == -1) {
		std::cout << "Неправильный ввод" << std::endl;
		return;
	}

	hypothesis = new char[pos + 1];
	for (int j = 0; j < pos; j++) {
		hypothesis[j] = temp[j];
	}
	hypothesis[pos] = '\0';

	conclusion = new char[ln - i + 1];
	for (int j = i; j < ln + 1; j++) {
		conclusion[j - i] = temp[j];
	}
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	char *hypo = nullptr, *concl = nullptr;
	EviTree solution;
	while (1) {
		input(hypo, concl);

		if (hypo == nullptr || concl == nullptr) {
			continue;
		}

		solution.SetHypo(hypo);
		solution.SetConcl(concl);

		std::cout << solution.GetHypo() << "|- " << solution.GetConcl() << std::endl;

	}
	


	system("pause");
	return 0;
}