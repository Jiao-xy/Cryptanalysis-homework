#include <iostream>
using namespace std;
void Affine_Analyse(string s)
{
    //仿射密码
    int len = s.length();
    cout << len << endl;
    double pinlv[30];
    int cishu[30];
    for (int i = 0; i < 30; i++)
    {
        cishu[i] = 0;
        pinlv[i] = 0.0;
    }

    for (int i = 0; i < len; i++)
    {
        char c = s[i];
        cishu[c - 'A']++;
    }
    double ans = 0;
    for (int i = 0; i < 26; i++)
    {
        pinlv[i] = double(cishu[i]) / len;
        cout << char('A' + i) << /*"出现次数:" << cishu[i] << "，频率为：" */"," << pinlv[i] << endl;
        ans += pinlv[i] * pinlv[i];
    }
    cout << "ans=" << ans << endl;

    int a[] = { 1,3,5, 7, 9,11,15,17,19,21,23,25 };
    int _a[] = { 1,9,21,15,3,19,7, 23,11,5, 17,25 };
    /*for (int i = 0; i < 12; i++)
        cout << a[i] * _a[i] % 26;*/
    while (1)
    {
        int x1, x2, y1, y2;
        cout << "依次输入x1,y1,x2,y2:";
        cin >> x1 >> y1 >> x2 >> y2;
        for (int i = 0; i < 12; i++)
        {

            int aa = a[i];
            for (int j = 0; j < 26; j++)
            {
                int tmp1 = (x1 * aa + j) % 26;
                int tmp2 = (x2 * aa + j) % 26;
                if (y1 == tmp1 && y2 == tmp2)
                {
                    cout << "密钥K(" << aa << "," << j << ")" << endl;
                    cout << "解密密钥K(" << _a[i] << "," << j << ")" << endl;
                    return;
                }
            }
        }
    }
}
string Affine_Encode(int a, int b, string s)//仿射密码加密函数
{
    int len = s.length();
    string C;
    for (int i = 0; i < len; i++)
    {
        C = C + char((a * int(s[i] - 'a') + b + 26) % 26 + 'a');
    }
    return C;
}
string Affine_Dncode(int a, int b, string s)//仿射密码解密函数
{
    int len = s.length();
    string P;
    for (int i = 0; i < len; i++)
    {
        P = P + char((a * int(s[i] - 'a' - b + 26)) % 26 + 'a');
    }
    return P;
}