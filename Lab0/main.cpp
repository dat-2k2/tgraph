#include"algo.h"
#include"generator.h"
#include"procedures.h"
#include <uchar.h>
#include<stdio.h>
#include<codecvt>
const char LIST[] = {'ï','ê','ÿ','÷','á','þ',' ','0','1','2','3','4','5','6','7','8','9','0','!','.'};
const int SIZE = 20;
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
	srand(time(NULL));
	cout << "Lab 0: Encoding, Decoding\n\n\n";
	cout << "Choose option:\n";
	cout << WRLE<< ". Encode, decode with RLE\n";
	cout << WFano<<". Encode, decode with Fano\n";
	cout << WRF <<". Encode, decode with RLE + Fano\n";
	cout << WFR << ". Encode, decode with Fano + RLE\n";
	cout << "5. New text\n";
	cout << "0. Exit\n\n\n";
	//generate textfile
	textgenerator(VOLUME, LIST, SIZE, TEXTFILE);
	int n;
	map<string, char>* decoder = 0;
	while (true) {
		n = input(n, [](int x) {if (x < 0 || x > 5) { std::cout << "Follow instruction!\n\n"; return false; }});
		switch (n) {
		case 0:
			return 0;
		case WRLE:
			encode(WRLE, TEXTFILE, ENCODEDFILE, decoder);
			decode(WRLE, ENCODEDFILE, CHECKFILE);
			if (check(TEXTFILE, CHECKFILE) == 0) cout << "Encoded successfully\n\n";
			else cout << "Encoded failed\n" << check(TEXTFILE, CHECKFILE) << "\n\n";
			compratio(TEXTFILE, ENCODEDFILE);
			break;
		case WFano:
			encode(WFano, TEXTFILE, ENCODEDFILE, decoder);
			decode(WFano, ENCODEDFILE, CHECKFILE, decoder);
			if (check(TEXTFILE, CHECKFILE) == 0) cout << "Encoded successfully\n\n";
			else cout << "Encoded failed\n" << check(TEXTFILE, CHECKFILE) << "\n\n";
			compratio(TEXTFILE, ENCODEDFILE);
			break;
		case WRF:
			encode(WRLE, TEXTFILE, TMPFILE, decoder);
			encode(WFano, TMPFILE, ENCODEDFILE, decoder);
			decode(WFano, ENCODEDFILE, TMPFILE, decoder);
			decode(WRLE, TMPFILE, CHECKFILE);
			if (check(TEXTFILE, CHECKFILE) == 0)
			{
				cout << "Encoded successfully\n\n";
				compratio(TEXTFILE, ENCODEDFILE);
			}
			else cout << "Encoded failed\n" << check(TEXTFILE, CHECKFILE) << "\n\n";
			break;
		case WFR:
			encode(WFano, TEXTFILE, TMPFILE, decoder);
			encode(WRLE, TMPFILE, ENCODEDFILE, decoder);
			decode(WRLE, ENCODEDFILE, TMPFILE);
			decode(WFano, TMPFILE, CHECKFILE, decoder);
			if (check(TEXTFILE, CHECKFILE) == 0)
			{
				cout << "Encoded successfully\n\n";
				compratio(TEXTFILE, ENCODEDFILE);
			}
			else cout << "Encoded failed\n" << check(TEXTFILE, CHECKFILE) << "\n\n";
			break;
		}
	}

	return 0;
}