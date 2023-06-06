#include<iostream>
#include<string>
#include <Windows.h> // for SetConsole
#include "redblack.cpp"
#include<fstream>
#include<wchar.h>
#include<vector>
#include <cctype>
#include"hashtable.cpp"
#include"config.h"
template<class T, class U> T input(T& n, U f, std::string alert = "Input type error !") {
	bool valid = false;
	while (!valid) {
		valid = true;
		std::cin >> n;
		if (std::cin.fail() || !(f(n))) {
			if (std::cin.fail()) std::cout << '\n'<<alert << '\n';
			std::cin.clear();
			std::cin.ignore();
			valid = false;
		}
	}
	return n;
}// f is a functor


template <class T> struct inrange {
private:
	T _low;
	T _high;
public:
	inrange(T l, T h) {
		_low = l, _high = h;
	}
	bool operator() (T n){
		return (_low <= n && n < _high);
	}
};


template <class T, class U> struct Pair {
	T key;
	U value;
#ifdef APPEND
	//must be for hash table
	bool operator==(const std::string key) {
		return key == key;
	}

	Pair<T, U>& operator+=(const Pair<T, U>& operand2) {
		if (key == operand2.key) {
			for (auto x : operand2.value) {
				value.push_back(x);
			}
		}
		return *this;
	}
#endif 

	Pair<T, U> operator=(const Pair<T, U>& other) {
		key = other.key;
		value = other.value;
	}
	bool operator> (const Pair<T, U> operand2) {
		return key > operand2.key;
	}
	bool operator< (const Pair<T, U> operand2) {
		return (*this != operand2 && !(*this > operand2));
	}
	bool operator== (const Pair<T, U> operand2) {
		return key == operand2.key;
	}

	bool operator!= (const Pair<T, U> operand2) {
		return !(*this == operand2);
	}



};

std::vector<std::string> split(std::string other) {
	std::vector<std::string> res;
	std::string tmp = "";
	for (char x : other) {
		bool isDelim = false;
		for (char check : "., ?:'`~\"/{}[]\n\t") {
			if (x == check) {
				res.push_back(tmp);
				tmp = "";
				isDelim = true;
				break;
			}
		}	
		if (!isDelim) {
			tmp.push_back(tolower(x));
		}
	}
	return res;
}

static RedBlackTree<Pair<std::string, std::vector<Pair<std::string, std::string>>>> rbtree;
static HashTable<std::string> table;
int main() {
	bool EXIT = false;

	//setting for Russian Symbols
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	while (!EXIT) {
		printf("=====================================\n");
		printf("Меню\n");
		printf("1. Поиск\n");
		printf("2. Добавить слово\n"); 
		printf("3. Удалить слово\n");
		printf("4. Удалить словарь\n");
		printf("5. Импортировать из файл\n");
		printf("0. Выход\n\n");

		int choice;
		choice = input(choice, inrange<int>(0, 6));

		if (!choice) {
			EXIT = true;
			continue;
		}

		switch (choice){
			case 1: {
				printf("Вводите слово: ");
				std::string query;
				query = input(query, [](auto x) {return true;});
				//TODO: find by word root
				int root_length = query.length() * 2 / 3;
				std::string root = query.substr(0, root_length);
				std::string tail = query.substr(root_length, query.length());


				printf("Искать на КЧ дереве...\n");
				auto res1 = rbtree.Search({ root,{{"",""}} });
				bool find1 = false;
				printf("\n");
				if (res1) {
					auto possible = res1->item->value;
					for (auto p : possible) {
						if (query == (res1->item->key + p.key)) {
							printf("Слово найдено:\n");
							printf("%s: %s\n", (res1->item->key + p.key).c_str(), p.value.c_str());
							find1 = true;
							break;
						}
					}	
				}
				if (!find1)
					printf("Такого слова нет!\n");

				printf("Искать в хеш-таблице...\n");
				auto res2 = table.Search(query);
				bool find2 = false;
				printf("\n");
				if (res2) {
					printf("Слово найдено:\n");
					printf("%s: %s\n", query.c_str(), res2->c_str());
					find2 = true;
					break;
				}
				if (!find2)
					printf("Такого слова нет!\n");
				break;
			}

			case 2: {
				std::string key;
				std::string value;
				printf("Вводите слово: ");
				key = input(key, [](auto x) {return true; });
				printf("Вводите значение: ");
				value = input(value, [](auto x) {return true; });
				
				int root_length = key.length() * 2 / 3;
				std::string root = key.substr(0, root_length);
				std::string tail = key.substr(root_length, key.length());

				rbtree.Insert({ root, { {tail, value} } });
				printf("Слово успешно добавлено на дерево\n");

				if(table.Insert(key, value))
					printf("Слово уже есть\n");
				else
					printf("Слово успешно добавлено в таблицу\n");

				break;
			}

			case 3: {
				std::string key;
				printf("Вводите слово: ");
				key = input(key, [](auto x) {return true; });

				int root_length = key.length() * 2 / 3;
				std::string root = key.substr(0, root_length);
				std::string tail = key.substr(root_length, key.length());

				if (!rbtree.Delete({ root, {{tail,""}} }))
					printf("Слово успешно удалено\n");
				else
					printf("Такого слова нет!");

				if (!table.Delete(key))
					printf("Слово успешно удалено\n");
				else
					printf("Такого слова нет!");
				break;
			}

			case 4: {
				rbtree.Clear();
				printf("Дерево удалено");

				table.Clear();
				printf("Таблица удалена");
				break;
			}

			case 5: {
				std::ifstream src;
				src.open("tolkien_return.txt", std::ios::binary);
				//text file often be read in short char mode, need to handle this situation.
				std::string line;
				char c;
				while (src.peek() != EOF){
					std::getline(src, line);
					auto listword = split(line);
					for (auto x : listword) {
						std::cout << x << " ";
					}

					std::cout << '\n';

					for (std::string word : listword) {
						int root_length = word.length() * 2 / 3;
						std::string root = word.substr(0, root_length);
						std::string tail = word.substr(root_length, word.length());
						rbtree.Insert({ root, {{tail, word}} });
						table.Insert(word, word);
					}
				}
				printf("Чтание успешно\n");
				src.close();
				break;
			}
			default:
				break;
		}
	}
}