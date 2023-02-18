#include"algo.h"
#include"generator.h"
#include"procedures.h"
#include <uchar.h>
#include<stdio.h>
#include<codecvt>
//const  char LIST[18];
const char test[] = { 'à','á','ñ','ä'};

template<class T, class U> T input(T& n, U f){
	bool valid = false;
	while (!valid) {
		valid = true; 
		cin >> n;
		if (cin.fail() || !(f(n))) {
			if (cin.fail()) cout << "Input type error !" << '\n';
			cin.clear();
			cin.ignore();
			valid = false;
		}
	}
	return n;
}

int main(){
	ofstream des;
	//generate textfile
	des.open(TEXTFILE);
	textgenerator(VOLUME, test, des);
	des.close();
	cout << "Lab 0: Encoding, Decoding\n\n\n";
	cout << "Choose option:\n";
	cout << WRLE<< ". Encode, decode with RLE\n";
	cout << WFano<<".Encode, decode with Fano\n";
	cout << WRFR <<".Encode, decode with RLE + Fano + RLE\n";
	cout << "0. Exit\n\n\n";
	int n;
	while (true) {
		n = input(n, [](int x) {if (x < 0 || x > 3) { std::cout << "Follow instruction!\n\n"; return false; }});
		if (n == 0) break;
		if (n != WRFR) {
			map<string, char> dictdecode = encode(n, TEXTFILE, ENCODEDFILE);
			decode(ENCODEDFILE, CHECKFILE, dictdecode);
		}
		else {
			encode(WRLE, TEXTFILE, "TMPD1.txt");
			decode("TMPD1.txt", CHECKFILE, map<string,char>());
			map<string, char> dictdecode = encode(WFano, "TMPD1.txt", "TMPD2.txt");
			decode("TMPD2.txt", CHECKFILE, dictdecode);
			encode(WRLE,"TMPD2.txt" , ENCODEDFILE);
			decode(ENCODEDFILE, CHECKFILE, map<string, char>());
		}
		decompratio(ENCODEDFILE, TEXTFILE);
	}

	return 0;
}