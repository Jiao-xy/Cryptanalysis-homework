#include "VigenereCipher.h"

string Vigenere_Encode(string s, string key)
{
	int len1 = s.length(), len2 = key.length();
	string c;
	for (int i = 0; i < len1; i++)
	{
		c = c + char((int(s[i] - 'a') + (key[i%len2]-'a') + 26) % 26 + 'a');
	}
	return c;
}

string Vigenere_Dncode(string s, string key)
{
	int len1 = s.length(), len2 = key.length();
	string m;
	for (int i = 0; i < len1; i++)
	{
		m = m + char((int(s[i] - 'a') - (key[i % len2] - 'a') + 26) % 26 + 'a');
	}
	return m;
}
