#pragma once
//run length algorithm
#include"config.h"
void RLE(ifstream& src, ofstream& des);
float Fano(ifstream& src, ofstream& des, map<string, char>*& decoder);
