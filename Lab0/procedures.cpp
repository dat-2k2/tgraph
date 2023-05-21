#include"procedures.h"
#include "algo.h"
int check(const char* f1, const char* f2) {
	ifstream src1, src2;
	src1.open(f1); src2.open(f2);
	int i = 0;
	while (!src1.eof() && !src2.eof()) {
		char tmp1, tmp2;
		src1.get(tmp1), src2.get(tmp2);
		if (tmp1 != tmp2) {
			cout << tmp1 << " " << tmp2 << "\n\n";
			return i;
		}
		i++;
	}
	if (!src1.eof() || !src2.eof()) return -1;
	return 0;
}
int decode(int mode, const char* src, const char* des, map<string, char>* dict) {
	ifstream f1; f1.open(src, ios::binary);
	ofstream f2; f2.open(des, ios::binary);
	if (check_opened(f1, f2) != 0) {
		return check_opened(f1,f2); //?
	}
	switch (mode) {
		case WRLE: {
			while (f1.peek() != EOF) {
				int f;
				char letter;
				char tmp;
				f1.get(letter); f1.get(tmp); f = int(tmp);
				while (f--) {
					f2.write(&letter, sizeof(char));
				}
				if (f1.eof()) break;
			}
			return 0;
		}
		case WFano: {
			string tmpstr = "";
			int indexinchar = 0;
			int paddingbits;
			string readin;
			char tmp;
			f1.seekg(0, std::ios::beg);
			while (f1.peek() != EOF) {
				f1.read(&tmp, 1);
				if (f1.peek() == EOF)
					cout << "";
				readin += tmp;
			}
			//save the number of paddling bits
			//unpacking
			paddingbits = int(readin.back());
			if (paddingbits == 8) paddingbits = 0;
			readin.pop_back();
			//take out the padding bit. 
			for (int i = 0; i < readin.size(); i++) {
				for (int j = 0; j < 8; j++) {
					//extract bit into char
					if (readin[i] & (1 << (7 - j))) {
						tmpstr += '1';
					}
					else tmpstr += '0';
				}
			}
			//clear the paddingbits
			while (paddingbits--) {
				tmpstr.pop_back();
			}
			string wordin = "";
			int index = 0;
			while (true) {
				wordin += tmpstr[index];
				if (dict->find(wordin) != dict->end()) {
					f2.write(reinterpret_cast<char*>(&((*dict)[wordin])), sizeof(char));
					wordin = "";
				}
				index++;
				if (index == tmpstr.size()) break;
			}
			return 0;
		}
	}
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
int encode(int choice, const char* src, const char* des, map<string, char>*& decoder){
	ifstream f1; f1.open(src, ios::binary);
	ofstream f2; f2.open(des, ios::binary);
	if (check_opened(f1, f2) != 0) {
		return check_opened(f1, f2);
	}	
	switch (choice) {
	case WRLE:
		RLE(f1, f2);
		return 0;
	case WFano:
		float p = Fano(f1, f2, decoder);
		printf("Цена кодирования %.2f\n\n", p);
		return 0;
	}
}

void compratio(const char* src, const char* des) { 
	ifstream f1; f1.open(src, ios::ate);
	ifstream f2; f2.open(des, ios::ate);
	printf("Compressing ratio %.2f\n", float(f1.tellg()) / float(f2.tellg()));
	printf("Data saving %.2f", 100 - float(f2.tellg()) / float(f1.tellg()) * 100);
	cout << "%\n\n";
}
	