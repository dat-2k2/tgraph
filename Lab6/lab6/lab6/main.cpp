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
#include"btree.cpp"
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

	bool operator> (const Pair<T, U> operand2) {
		return key > operand2.key;
	}
	bool operator< (const Pair<T, U> operand2) {
		return (*this != operand2 && !(*this > operand2));
	}
	bool operator>= (const Pair<T, U> operand2) {
		return (key > operand2.key) || (key == operand2.key);
	}
	bool operator<= (const Pair<T, U> operand2) {
		return (key < operand2.key) || (key == operand2.key);
	}
	bool operator== (const Pair<T, U> operand2) {
		return key == operand2.key;
	}

	bool operator!= (const Pair<T, U> operand2) {
		return !(*this == operand2);
	}

	friend std::ostream& operator<< (std::ostream& os, const Pair<T, U>& other) {
#ifdef APPEND
		os << "[";
		int i = 0;
		for (; i < other.value.size()-1; i++) {
			os << (other.key + other.value[i]) << ", ";
		}
		os << other.key + other.value[i] << "]";

#else
		os << other.key;
#endif
		return os;
	}
};

const int L = 33;
const char ALPHABET1[L] = { '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�' };
const char ALPHABET2[L] = {'�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�'};

char convert(char x) {
	for (int i = 0; i < L; i++) {
		if (ALPHABET1[i] && x == ALPHABET1[i])
			return ALPHABET1[i];
	}
	for (int i = 0; i < L;i++) {
		if (ALPHABET2[i] && x == ALPHABET2[i])
			return ALPHABET1[i];
	}
	return 0;
}
std::vector<std::string> split(std::string other) {
	std::vector<std::string> res;
	std::string tmp;
	for (char x : other) {
		x = convert(x);
		if (x) {
			tmp.push_back(x);
		}
		else {
			if (tmp.length()) {
				res.push_back(tmp);
			}
			tmp = std::string();
		}
	}
	return res;
}

static RedBlackTree<Pair<std::string, std::vector<std::string>>> rbtree;
static HashTable table{13};
static BTree<Pair<std::string, std::vector<std::string>>> btree;
int main() {
	bool EXIT = false;
	bool EXITMENU = false;
	//setting for Russian Symbols
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	while (!EXIT) {
		printf("=====================================\n");
		printf("����\n");
		printf("1. ������-������ ������\n");
		printf("2. ���-�������\n");
		printf("3. B+ ������\n");
		printf("0. �����\n\n");
		int mode;
		mode = input(mode, inrange<int>(0, 4));
		if (!mode) {
			EXIT = true;
			break;
		}
		else {
			EXITMENU = false;
		}
		switch (mode) {
			case 1: {
				while (!EXITMENU) {
					printf("=====================================\n");
					printf("����\n");
					printf("1. �����\n");
					printf("2. �������� �����\n");
					printf("3. ������� �����\n");
					printf("4. ������� �������\n");
					printf("5. ������������� �� ����\n");
					printf("6. ������������ ������\n");
					printf("7. ���������� ������������ (�������� ������)\n");
					printf("0. ����� � ����\n\n");

					int choice;
					choice = input(choice, inrange<int>(0, 8));

					if (!choice) {
						EXITMENU = true;
						continue;
					}

					switch (choice) {
					case 1: {
						printf("������� �����: ");
						std::string query;
						query = input(query, [](auto x) {return true; });
						//TODO: find by word root
						int root_length = floor(float(query.length()) * 2. / 3.);
						std::string root = query.substr(0, root_length);
						std::string tail = query.substr(root_length , query.length());


						printf("������ �� �� ������...\n\n");
#ifdef APPEND
						auto res1 = rbtree.Search({ root,{{"",""}} });
						bool find1 = false;
						printf("\n");
						if (res1) {
							//subkey
							for (auto p: res1->_item.value) {
								if (p == tail) {
									printf("����� �������: %s\n", query.c_str());
									printNodeInfo(res1);
									find1 = true;
									break;
								}
							}
						}
						if (!find1)
							printf("������ ����� ���!\n");
#else
						auto res1 = rbtree.Search({ query,{} });
						bool find1 = false;
						printf("\n");
						if (res1) {
							printf("����� �������: %s\n", query.c_str());
							printNodeInfo(res1);
						}
						else
							printf("������ ����� ���!\n");
#endif // APPEND


						break;
					}

					case 2: {
						std::string key;
						printf("������� �����: ");
						key = input(key, [](auto x) {return true; });

						int root_length = floor(float(key.length()) * 2. / 3.);
						std::string root = key.substr(0, root_length);
						std::string tail = key.substr(root_length, key.length());
#ifdef APPEND

						auto res1 = rbtree.Search({ root,{{"",""}} });
						bool find1 = false;
						printf("\n");
						if (res1) {
							//subkey
							for (auto p : res1->_item.value) {
								if (p == tail) {
									printf("����� ��� ����!\n");
									printNodeInfo(res1);
									find1 = true;
									break;
								}
							}
						}
						if(!find1) {
							rbtree.Insert({ root, {tail} });
							printf("����� ������� ��������� �� ������\n");
							res1 = rbtree.Search({ root,{} });
							printNodeInfo(res1);
						}
#else
						auto res1 = rbtree.Search({ key,{} });
						if (res1) {
							printf("����� ��� ����!\n");
							printNodeInfo(res1);
						}
						else {
							rbtree.Insert({ key , {} });
							printf("����� ������� ��������� �� ������\n");
							res1 = rbtree.Search({ key,{} });
							printNodeInfo(res1);
						}
					
#endif // APPEND

						break;
					}

					case 3: {
						std::string key;
						printf("������� �����: ");
						key = input(key, [](auto x) {return true; });
						int root_length = floor(float(key.length()) * 2. / 3.);
						std::string root = key.substr(0, root_length);
						std::string tail = key.substr(root_length, key.length());
#ifdef APPEND
						auto n = rbtree.Search({ root, {tail} });
						if (n) {
							auto it_subkey = std::find(n->_item.value.begin(), n->_item.value.end(), tail);
							if (it_subkey != n->_item.value.end()) {
								if (n->_item.value.size() > 1)
									n->_item.value.erase(it_subkey);
								else
									rbtree.Delete({ root,{tail} });
								printf("����� ������� �������\n");
								break;
							}
						}
						printf("������ ����� ���!\n");
#else
						if (!rbtree.Delete({ key, {} }))
							printf("����� ������� �������\n");
						else
							printf("������ ����� ���!\n");
#endif // 
						break;
					}

					case 4: {
						rbtree.Clear();
						printf("������ �������\n");
						break;
					}

					case 5: {
						std::ifstream src;
						src.open("tolkien_return.txt", std::ios::binary);
						//text file often be read in short char mode, need to handle this situation.
						std::string line;
						char c;
						while (src.peek() != EOF) {
							std::getline(src, line);
							auto listword = split(line);
							for (auto x : listword) {
								std::cout << x << " ";
							}

							std::cout << '\n';

							for (std::string word : listword) {
								int root_length = floor(float(word.length()) * 2. / 3.);
								std::string root = word.substr(0, root_length);
								std::string tail = word.substr(root_length, word.length());
#ifdef APPEND
								rbtree.Insert({ root,{tail} });

#else
								rbtree.Insert({ word,{} });
#endif // APPEND
							}
						}
						printf("������ �������\n");
						src.close();
						break;
					}

					case 6: {
						rbtree.visual();
						break;
					}

					case 7: {
						if (rbtree.Root == nullptr) {
							printf("������!\n");
						}
						else
						rbtree.printInorder(rbtree.Root);
						printf("\n");
					}

					default:
						break;
					}
				}
			}
			case 2: {
				while (!EXITMENU) {
					printf("=====================================\n");
					printf("����\n");
					printf("1. �����\n");
					printf("2. �������� �����\n");
					printf("3. ������� �����\n");
					printf("4. ������� �������\n");
					printf("5. ������������� �� ����\n");
					printf("6. ������������ �������\n");
					printf("0. ����� � ����\n\n");

					int choice;
					choice = input(choice, inrange<int>(0, 7));

					if (!choice) {
						EXITMENU = true;
						continue;
					}

					switch (choice) {
					case 1: {
						printf("������� �����: ");
						std::string query;
						query = input(query, [](auto x) {return true; });

						printf("������ � ���-�������...\n");
						auto res2 = table.Search(query);
						bool find2 = false;
						printf("\n");
						if (res2) {
							printf("����� �������: %s\n", query.c_str());
							printf("��������� �����: %s\n", (res2->_next) ? (res2->_next->_key.c_str()) : "None");
							find2 = true;
							break;
						}
						if (!find2)
							printf("������ ����� ���!\n");
						break;
					}

					case 2: {
						std::string key;
						std::string value;
						printf("������� �����: ");
						key = input(key, [](auto x) {return true; });

						auto inserted_item = table.Insert(key);
						if (inserted_item) {
							printf("����� ������� ��������� � �������\n");
							table.printRow(inserted_item);
						}
						else {
							printf("����� ��� ����\n");
							auto find_item = table.Search(key);
							table.printRow(find_item);
						}
							
						break;
					}

					case 3: {
						std::string key;
						printf("������� �����: ");
						key = input(key, [](auto x) {return true; });

						if (!table.Delete(key))
							printf("����� ������� �������\n");
						else
							printf("������ ����� ���!\n");
						break;
					}

					case 4: {
						table.Clear();
						printf("������� �������\n");
						break;
					}

					case 5: {
						std::ifstream src;
						src.open("tolkien_return.txt", std::ios::binary);
						//text file often be read in short char mode, need to handle this situation.
						std::string line;
						char c;
						while (src.peek() != EOF) {
							std::getline(src, line);
							auto listword = split(line);
							for (auto x : listword) {
								std::cout << x << " ";
							}

							std::cout << '\n';

							for (std::string word : listword) {
								table.Insert(word);
							}
						}
						printf("������ �������\n");
						src.close();
						break;
					}

					case 6: {
						table.visual();
						break;
					}
					default:
						EXITMENU = true;
						break;
					}
				}
			}
			case 3: {
				while (!EXITMENU) {
					printf("=====================================\n");
					printf("����\n");
					printf("1. �����\n");
					printf("2. �������� �����\n");
					printf("3. ������� �����\n");
					printf("4. ������� �������\n");
					printf("5. ������������� �� ����\n");
					printf("6. ������������ ������\n");
					printf("7. ���������� ������������ (�������� ������)\n");
					printf("0. ����� � ����\n\n");

					int choice;
					choice = input(choice, inrange<int>(0, 8));

					if (!choice) {
						EXITMENU = true;
						continue;
					}

					switch (choice) {
					case 1: {
						printf("������� �����: ");
						std::string query;
						query = input(query, [](auto x) {return true; });
						//TODO: find by word root
						int root_length = floor(float(query.length()) * 2. / 3.);
						std::string root = query.substr(0, root_length);
						std::string tail = query.substr(root_length, query.length());


						printf("������ �� B+ ������...\n\n");
#ifdef APPEND
						//only find to the main key 
						auto n = btree.search({ root, {} });
						if (n) {
							int index = n->find_index({ root,{} });
							//find subkey
							auto it_subkey = std::find(n->_item[index].value.begin(), n->_item[index].value.end(), tail);
							if (it_subkey != n->_item[index].value.end()) {
								printf("����� �������: %s\n", query.c_str());
								printNodeInfo(n);
								break;
							}
						}
							printf("������ ����� ���!\n");
#else
						auto res1 = btree.search({ query,{} });
						bool find1 = false;
						printf("\n");
						if (res1) {
							printf("����� �������: %s\n", query.c_str());
							printNodeInfo(res1);
						}
						else
							printf("������ ����� ���!\n");
#endif // APPEND


						break;
					}

					case 2: {
						std::string key;
						printf("������� �����: ");
						key = input(key, [](auto x) {return true; });

						int root_length = floor(float(key.length()) * 2. / 3.);
						std::string root = key.substr(0, root_length);
						std::string tail = key.substr(root_length, key.length());
#ifdef APPEND
						//only add to the subkey vector.
						auto n = btree.search({ root, {} });
						if (n) {
							int index = n->find_index({ root,{} });
							auto it_subkey = std::find(n->_item[index].value.begin(), n->_item[index].value.end(), tail);
							if (it_subkey != n->_item[index].value.end()) {
								std::cout << "����� " << key << " ��� ���� �� �������\n";
								break;
							}
							else {
								n->_item[index].value.push_back(tail);
								printf("����� ������� ��������� �� ������\n");
							}
						}
#else
						auto res1 = btree.search({ key,{} });
						if (res1) {
							printf("����� ��� ����!\n");
							printNodeInfo(res1);
						}
						else {
							btree.insert({ key , {} });
							printf("����� ������� ��������� �� ������\n");
							res1 = btree.search({ key,{} });
							printNodeInfo(res1);
						}

#endif // APPEND

						break;
					}

					case 3: {
						std::string key;
						printf("������� �����: ");
						key = input(key, [](auto x) {return true; });

						int root_length = floor(float(key.length()) * 2. / 3.);
						std::string root = key.substr(0, root_length);
						std::string tail = key.substr(root_length, key.length());
#ifdef APPEND
						auto n = btree.search({ root, {tail} });
						if (n) {
							int index = n->find_index({ root, {tail} });
							auto it_subkey = std::find(n->_item[index].value.begin(), n->_item[index].value.end(), tail);
							if (it_subkey != n->_item[index].value.end()) {
								//after delete remains subkey
								if (n->_item[index].value.size() > 1)
									n->_item[index].value.erase(it_subkey);
								else {
									//need to delete the node
									btree.visual();
									btree.remove({ root,{} });
								}
								printf("����� ������� �������\n");
								break;
							}
						}
						printf("������ ����� ���!\n");
#else
						if (!btree.remove({ key, {} }))
							printf("����� ������� �������\n");
						else
							printf("������ ����� ���!\n");
#endif // 
						break;
					}

					case 4: {
						btree.clear();
						printf("������ �������\n");
						break;
					}

					case 5: {
						std::ifstream src;
						src.open("tolkien_return.txt", std::ios::binary);
						//text file often be read in short char mode, need to handle this situation.
						std::string line;
						char c;
						while (src.peek() != EOF) {
							std::getline(src, line);
							auto listword = split(line);
							for (auto x : listword) {
								std::cout << x << " ";
							}

							std::cout << '\n';

							for (std::string word : listword) {
								int root_length = floor(float(word.length()) * 2. / 3.);
								std::string root = word.substr(0, root_length);
								std::string tail = word.substr(root_length, word.length());
#ifdef APPEND
								//only find main key 
								auto n = btree.search({ root, {tail} });
								if (n) {
									int index = n->find_index({ root, {tail} });
									auto it_subkey = std::find(n->_item[index].value.begin(), n->_item[index].value.end(), tail);
									if (it_subkey != n->_item[index].value.end()) {
										std::cout << "����� " << word << " ��� ���� �� �������\n";
										continue;
									}
									else {
										n->_item[index].value.push_back(tail);
										break;
									}
								}
								else
									// if main key doesn't exist, add new item
									btree.insert({ root,{tail} });
#else
								if (btree.search({ word,{} })) {
									std::cout << "����� " << word << " ��� ���� �� �������\n";
								}
								else {
									btree.insert({ word,{} });
								}
#endif // APPEND
							}
						}
						printf("������ �������\n");
						src.close();
						break;
					}

					case 6: {
						btree.visual();
						break;
					}

					case 7: {
						if (btree.Root == nullptr) {
							printf("������!\n");
						}
						else
							btree.printInOrder();
						printf("\n");
					}

					default:
						break;
					}
				}
			}
		}
		
	}
}