#include <string>
#include <iostream>

using namespace std;

bool findStringNaive(string pattern, string text)
{
	unsigned int j = 0;

	for (unsigned int i = 0; i < text.size(); i++)
	{
		if (pattern[j] != text[i])
			j = 0;
		else{
			if (j == pattern.size() - 1)
				return true;
			j++;
		}
	}

	return false;
}

void preKMP(string pattern, int f[])
{
	int m = pattern.size(), k;
	f[0] = -1;
	for (int i = 1; i < m; i++)
	{
		k = f[i - 1];
		while (k >= 0)
		{
			if (pattern[k] == pattern[i - 1])
				break;
			else
				k = f[k];
		}
		f[i] = k + 1;
	}
}

bool KMP(string pattern, string text)
{
	int m = pattern.size();
	int n = text.size();
	int f[m];

	preKMP(pattern, f);

	int i = 0;
	int k = 0;
	while (i < n)
	{
		if (k == -1)
		{
			i++;
			k = 0;
		}
		else if (text[i] == pattern[k])
		{
			i++;
			k++;
			if (k == m)
				return true;
		} else
			k = f[k];
	}
	return false;
}

int smallest(int x, int y, int z){
    return min(min(x, y), z);
}

int EditDistance(string pattern, string text)
{
	// inicialização
	int P = pattern.size();
	int T = text.size();
	int D[P+1][T+1];

	for (int i = 0; i <= P; i++)
		D[i][0] = i;
	for (int j = 0; j <= T; j++)
		D[0][j] = j;

	// recorrência
	for (int i = 1; i <= P; i++){
		for (int j = 1; j <= T; j++){
			if (pattern[i-1] == text[j-1])
				D[i][j] = D[i-1][j-1];
			else D[i][j] = 1 + smallest(D[i-1][j-1], D[i-1][j], D[i][j-1]);
		}
	}
	return D[P][T];
}
