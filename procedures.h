#pragma once
#include"config.h"
map<string, char> encode(int choice, const char* src, const char* des);
void decode(const char* src, const char* des, map<string, char>dict); int check(ifstream& src1, ifstream& src2);
int check_opened(ifstream& src, ofstream& des);
void decompratio(const char* src, const char* des);