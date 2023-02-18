#include"procedures.h"
#include "algo.h"
int check(ifstream& src1, ifstream& src2) {
	while (true) {
		char tmp1, tmp2;
		src1.get(tmp1), src2.get(tmp2);
		if (!(src1.eof() ^ src2.eof())) return -1;
		if (tmp1 != tmp2) return 1;
	}
	return 0;
}
void decode(const char* src, const char* des, map<string, char>dict) {
	ifstream f1; f1.open(src);
	ofstream f2; f2.open(des);
	if (check_opened(f1, f2) != 0) return; //?
	if (dict.empty()) {
		//RLE mode
		while (f1.peek() != EOF) {
			int f; 
			char letter;
			char tmp; 
			f1.get(letter); f1.get(tmp); f = int(tmp);
			if (f1.eof()) break;
			while (f--) {
				f2.write(&letter, sizeof(char));
			}
		}
		f2.close();
		ifstream f3; f3.open(des);
		if (check(f1, f3)) cout << "Encoded successfully\n\n";
		else cout << "Encoded failed\n\n";
		return;
	}
	string tmpstr = "";
	while (f1.peek() != EOF) { 
		char tmp;
		f1.get(tmp);
		tmpstr += tmp;
		if (dict.find(tmpstr) != dict.end()) {
			f2.write(reinterpret_cast<char*>(&dict[tmpstr]), sizeof(char) );
			tmpstr = "";
		}
	}
	f2.close();
	ifstream f3; f3.open(des);
	if (check(f1, f3)) cout << "Encoded successfully\n\n";
	else cout << "Encoded failed\n\n";
}

int check_opened(ifstream& src, ofstream& des) {
	int x = 0;
	if (!src.is_open()) {
		cout << "Error: Cannot open source file!\n\n";
		x = SOURCEFILECANNOTOPEN;
	}
	if (!des.is_open()) {
		cout << "Error: Cannot open destination file!\n\n";
		x = DESFILECANNOTOPEN;
	}
	return x;
}
map<string, char> encode(int choice, const char* src, const char* des) {
	ifstream f1; f1.open(src);
	ofstream f2; f2.open(des);
	if (check_opened(f1, f2) != 0) return map<string, char>(); //?
	switch (choice) {
	case WRLE:
		return RLE(f1, f2);
	case WFano:
		return Fano(f1, f2);
	}
}

void decompratio(const char* src, const char* des) {
	ifstream f1; f1.open(src, ios::ate);
	ifstream f2; f2.open(des, ios::ate);
	cout << "Compression rate " << float(f2.tellg()) / float(f1.tellg() * 100)<<"% \n\n";
}
	