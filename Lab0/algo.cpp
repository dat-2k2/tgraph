#include"algo.h"
#include<bitset>
void RLE(ifstream& src, ofstream& des) {
	//read by group of symbol
	char current;
	int Count= 1;
	src.get(current);
	while (true){
		char tmp;
		//read until symbol changes 
		while (src.peek()!= EOF) {
			src.get(tmp);
			if (tmp == current) Count++; 
			else {
				break;
			}
			if (Count == CHAR_MAX) break;
		};
		//write the encoded to des
			des.write(&current, sizeof(char));
			des.write(reinterpret_cast<char*>(&Count), sizeof(char));
		//reset
		Count = 1;
		//if not be eof, then continue
		if (!src.eof()) {
			current = tmp;
		}
		else break;
	}
}

void Fanorecursive(vector<pair<char, int>>& alphabet, int SUM, int start, int end, vector<string>& encode) {
	if (start >= end) return;
	if (end - start + 1 >= 2) {
		//find pivot
		int partsum = 0;
		int pivot = start;
		int partsumtmp = 0;
		while (partsum * 2 - SUM <= 0 && pivot < end) {
			partsum += alphabet[pivot++].second;
		}
		if ((partsum*2 - SUM ) > (SUM  + alphabet[pivot-1].second*2 - partsum*2)) {
			partsum -= alphabet[--pivot].second;
		}
		//encode 2 parts
		for (int i = start; i < pivot; i++) {
			encode[i] += '0';
		}
		for (int i = pivot; i <= end; i++) {
			encode[i] += '1';
		}
		//regression
		Fanorecursive(alphabet, partsum, start, pivot-1, encode);
		Fanorecursive(alphabet, SUM - partsum, pivot, end, encode);
	}
	else {
		return;
	}
};

float Fano(ifstream& src, ofstream& des, map<string, char>*& decoder) {
	vector<pair<char, int>> alphabet;
	int SUM = 0;
	while (true){
		char tmp;
		src.get(tmp);
		if (src.eof()) break;
		SUM++;
		//check coincidence
		int i;
		for (i = 0; i < alphabet.size(); i++) {
			if (alphabet[i].first == tmp) {
				alphabet[i].second++;
				break;
			}
		}
		if (i == alphabet.size()) {
			alphabet.push_back(pair<char, int>(tmp, 1));
		}
	}
	sort(alphabet.begin(), alphabet.end(), [](pair<char, int> x, pair<char, int>y) {return (x.second > y.second); });
	vector<string>encode(alphabet.size());
	if (alphabet.size() < 2) {
		encode[0] = '1';
	}
	else {
		Fanorecursive(alphabet, SUM, 0, alphabet.size() - 1, encode);
	}

	map<char, string> encoder;
	decoder = new map<string, char>;
	//simultaneously calculate цену кодирования 
	float price = 0;
	for (int i = 0; i < alphabet.size(); i++) {
		encoder[alphabet[i].first] = encode[i];
		//price = sum (freq* lengthof(codeword))
		price += (float)(alphabet[i].second) / SUM * encode[i].size();
		(*decoder)[encode[i]] = alphabet[i].first;
	}
	

	//back to the start
	src.clear();
	src.seekg(0, std::ios::beg);

	//--------------------------------------------
	//bit packing
	vector<char>packed;
	packed.push_back(0);
	int counter = 0;
	int track = 0;
	//bit packing directly from file
	while (true) {
		char input;
		src.get(input);
		if (src.eof()) break;
		//write by bit
		for (int j = 0; j < encoder[input].size(); j++) {
			if (counter >= 8) {
				packed.push_back(0);
				counter = 0;
			}
			if (encoder[input][j] == '1') {
				packed.back() |= (1 << (7 - counter));
			}
			counter++;
		}
	}
	//last character tells how many "paddling" bits at the end of file - a flag
	if (counter < 8) {
		packed.push_back(8 - counter);
	}
	else {
		packed.push_back(8);
	}
	//---------------------------------------------------
	
	//write to file
	for (int i = 0; i < packed.size(); i++) {
		des.write(&packed[i], sizeof(char));
	}

	//return цена кодирования 
	return price;
}