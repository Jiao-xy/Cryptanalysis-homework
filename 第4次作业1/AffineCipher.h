#pragma once
#include <iostream>
using namespace std;
#ifndef AFFINECIPHER_H
#define AFFINECIPHER_H
void Affine_Analyse(string s);
string Affine_Encode(int a, int b, string s);
string Affine_Dncode(int a, int b, string s);

#endif
