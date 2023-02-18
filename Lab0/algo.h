#pragma once
//run length algorithm
#include"config.h"
map<string, char> RLE(ifstream& src, ofstream& des);
void Huffman(ifstream& src, ofstream& des);
map<string,char> Fano(ifstream& src, ofstream& des);

//need to return simultaneously a dictionary map<char,string>