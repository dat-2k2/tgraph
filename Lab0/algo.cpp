#include"algo.h"
map<string, char>RLE(ifstream& src, ofstream& des) {
	//read by group of symbol
	char current;
	int Count = 0;
	src.get(current);
	while (src.peek() != EOF){
		char tmp;
		//read until symbol changes 
		do {
			src.get(tmp);
			if (src.eof()) break;
			Count++;
			if (Count == CHAR_MAX) break;
		} while (tmp == current && src.peek() != EOF);
		//write the encoded to des
		des.write(&current, sizeof(char));
		des.write(reinterpret_cast<char*>(&Count), sizeof(char));
		//reset
		Count = 0;
		//if not be eof, then continue
		if (src.peek() != EOF) {
			current = tmp;
		}
	}
	return map<string, char>();
}

//tree implementation
struct node {
	char name;
	int freq;
	node* left;
	node* right;
};
void Huffman(ifstream& src, ofstream& des) {
	//preprocess
	vector<pair<char, int>> alphabet;
	while (true) {
		char tmp;
		src.get(tmp);
		if (src.eof()) break;
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
	// now we got a frequency-descending alphabet 
	//Huffman: construct a tree from below to top
	while (!alphabet.empty()) {
		node* tmp1 = new node;
		node* tmp2 = new node;
		tmp1->name = alphabet.end()->first;
		tmp1->freq = alphabet.end()->second;
		alphabet.pop_back();
		tmp1->name = alphabet.end()->first;
		tmp2->freq = alphabet.end()->second;
		alphabet.pop_back();
		alphabet.push_back(pair<char, int>(tmp1->name + tmp2->name, tmp1->freq+tmp2->freq));
	}
}

void subFano(vector<pair<char, int>>& alphabet, int SUM, int start, int end, vector<string>& encode) {
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
		subFano(alphabet, partsum, start, pivot-1, encode);
		subFano(alphabet, SUM - partsum, pivot, end, encode);
	}
	else {
		return;
	}
};
map<string, char> Fano(ifstream& src, ofstream& des) {
	vector<pair<char, int>> alphabet;
	int SUM = 0;
	while (true) {
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
		encode[0] = '0';
	}
	else {
		subFano(alphabet, SUM, 0, alphabet.size() - 1, encode);
	}
	//create dict
	map<char, string>dictencode;
	for (int i = 0; i < alphabet.size(); i++) {
		dictencode[alphabet[i].first] = encode[i];
	}
	//write to the dest file
	//back to the start
	src.clear();
	src.seekg(0, std::ios::beg);
	while (true) {
		char tmp;
		src.get(tmp);
		if (src.eof()) break;
		des.write(dictencode[tmp].c_str(), dictencode[tmp].size());
	}
	map<string, char> dictdecode;
	for (int i = 0; i < alphabet.size(); i++) {
		dictdecode[encode[i]] = alphabet[i].first;
	}
	return dictdecode;
}