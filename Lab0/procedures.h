#pragma once
#include"config.h"
int encode(int choice, const char* src, const char* des, map<string, char>*& decoder);
int decode(int mode, const char* src, const char* des, map<string, char>* decoder = 0);
int check_opened(ifstream& src, ofstream& des);
void compratio(const char* src, const char* des);
int check(const char* f1, const char* f2);