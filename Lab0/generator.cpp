#include"generator.h"

void textgenerator(int volume, const char datainp[], const int size, const char* File){
	ofstream f;
	f.open(File);
	if (!f.is_open()) {
		cout << "Creating file failed\n\n";
		return;
	}
	for (int i = 0; i < volume; i++) {
		f.write(&datainp[rand() % size], sizeof(char));
	}
	std::cout << "New text file created successfully\n\n";
}