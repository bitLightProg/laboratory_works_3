#include <string>
#include <list>
#include <iostream>
#include <Windows.h>


class ParseTree {
public:
	ParseTree(): parent(nullptr){}
	~ParseTree() {}

	void AddChild(std::string &s) {
		ParseTree *ch = new ParseTree;
		ch->SetString(s);
		ch->SetParent(this);
		children.push_back(*ch);
	}

	void SetString(std::string s) {
		str = s;
	}

	std::string GetString() {
		return str;
	}

	void SetParent(ParseTree *p) {
		parent = p;
	}

	ParseTree* GetChild(int n) {
		std::list<ParseTree>::iterator it = children.begin();
		while (n-- > 0)
		{
			if (it != children.end())
				++it;
			else break;
		}
		if (n > 0 || it == children.end())
			return nullptr;
		return &(*it);
	}
private:
	ParseTree *parent;
	std::list<ParseTree> children;
	std::string str;
};
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
std::string GetInput() {
	std::cout << "Введите высказывание в виде: not a and not (b or c) -> d, a |- a ;"
		<< "a |- (составные знаки должны быть нераздельны)." << std::endl;

	std::string result;
	std::getline(std::cin, result);
	return result;
}
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

	int i, ln = strlen(temp), count1 = 0, count2 = 0, pos = -1;
	for (i = 0; i < ln; ++i) {
		if (temp[i] == ';')
			break;

		if (temp[i] == '|' && temp[i + 1] == '-') {
			++i;
			pos = -2;
			continue;
		}

		if (temp[i] != ' ') {
			if (pos == -1)
				count1++;
			else if (pos == -2) {
				pos = i;
				count2++;
			}
			else count2++;
		}

		/*if (pos == -1 && temp[i] != ' ')
			count1++;
		if (pos != -1 && temp[i] != ' ')
			pos = i;

		if (temp[i] != ' ')
			count2++;*/
	}

	if (pos == -1) {
		std::cout << "Неправильный ввод" << std::endl;
		return;
	}

	hypothesis = new char[count1 + 1];
	int j = 0, k = 0;
	for (; count1 > 0; j++) {
		if (temp[j] != ' ') {
			hypothesis[k++] = temp[j];
			count1--;
		}
	}
	hypothesis[k] = '\0';

	conclusion = new char[count2 + 1];
	for (j = 0, k = 0; count2 > 0; j++) {
		if (temp[pos + j] != ' ') {
			conclusion[k++] = temp[pos + j];
			count2--;
		}
	}
	conclusion[k] = '\0';
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	ParseTree tr;
	EviTree solution;
	ParseTree *iter = &tr;
	while (1) {
		std::string str = GetInput();
		std::cout << str << std::endl;
		
		while ((iter->GetString()).size() > 0) {
			iter = iter->GetChild(0);
		}
		iter->SetString(str);
		iter->AddChild(std::string(""));
		/*if (hypo == nullptr || concl == nullptr) {
			continue;
		}

		solution.SetHypo(hypo);
		solution.SetConcl(concl);

		std::cout << solution.GetHypo() << "|-" << solution.GetConcl() << std::endl;*/

	}
	


	system("pause");
	return 0;
}