#include"generator.h"

int rand_num(int range) {
	return rand() / (1 + RAND_MAX) * range;
}
void textgenerator(int volume, const char datainp[], ofstream& File){
	srand(time(NULL));
	for (int i = 0; i < volume; i++) {
		File.write(&datainp[rand() % sizeof(datainp)], sizeof(char));
	}
}