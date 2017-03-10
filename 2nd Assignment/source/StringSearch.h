#include <iostream>
#include <string>

using namespace std;

bool findStringNaive(string pattern, string text);

void preKMP(string pattern, int f[]);

bool KMP(string pattern, string text);

int smallest(int x, int y, int z);

int EditDistance(string pattern, string text);
