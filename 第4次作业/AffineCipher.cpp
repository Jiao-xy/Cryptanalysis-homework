#include <iostream>
using namespace std;
string Affine_Encode(int a, int b, string s)//·ÂÉäÃÜÂë¼ÓÃÜº¯Êı
{
    if (a == 0 || a % 2 == 0) {
        return "Error";
    }
    int len = s.length();
    string C;
    for (int i = 0; i < len; i++)
    {
        C = C + char((a * int(s[i] - 'a') + b + 26) % 26 + 'a');
    }
    return C;
}
string Affine_Dncode(int a, int b, string s)//·ÂÉäÃÜÂë½âÃÜº¯Êı
{
    if (a == 0 || a % 2 == 0) {
        return "Error";
    }
    int len = s.length();
    string P;
    for (int i = 0; i < len; i++)
    {
        P = P + char((a * int(s[i] - 'a' - b + 26)) % 26 + 'a');
    }
    return P;
}