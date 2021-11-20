// 密码分析.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include<algorithm>
#include"AffineCipher.h"
#include"VigenereCipher.h"
using namespace std;
FILE* fp;
FILE* fw;
double rate[26] = { 0.082,0.015,0.028,0.043,0.127,0.022,0.02,0.061,0.07,0.002,0.008,0.04,0.024,0.067,0.075,0.019,0.001,0.060,0.063,0.091,0.028,0.01,0.023,0.001,0.02,0.001 };
double _rate[26];//密文中各个字母出现的概率
int Fre[26];//字母出现频数
string str2lower(string s)
{
	//大写转小写
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] >= 'A' && s[i] <= 'Z')s[i]+=32;
	}
	return s;
}
int gcd(int a, int b) {
	//计算最大公约数
	if (b == 0) return a;
	return gcd(b, a % b);
}
double Mg(string s, bool m=false)
{
	//计算明文指数或重合指数，第二个参数默认缺省为false，若m为true，则计算重合指数，否则计算明文指数。
	int len = s.length();
	double mg = 0;
	memset(Fre, 0, sizeof(Fre));
	for (int i = 0; i < len; i++)
	{
		Fre[s[i] - 'a']++;
	}
	
	for (int i = 0; i < 26; i++)
	{
		_rate[i] = (double)Fre[i] / (double)len;
		
	}
	if (m)//计算重合指数
	{
		for (int i = 0; i < 26; i++)
		{
			mg = mg + (_rate[i] * (((double)Fre[i]-1) / ((double)len-1)));
		}
		return mg;
	}
	for (int i = 0; i < 26; i++)
	{
		mg = mg + ((double)Fre[i] / (double)len) * rate[i];
	}
	return mg;
}
int Kasiski(string s)
{
	int m = 0;
	int len = s.length();
	for (int i = 0; i < len - 2; i++)
	{
		string chr = "";
		chr = chr + s[i] + s[i + 1] +s[i + 2];
		int c = 0;//记录出现次数
		int index[100];
		memset(index, 0, sizeof(index));
		for (int j = 0; j < len - 2; j++)
		{
			string tmp = "";
			tmp = tmp + s[j] + s[j + 1] +s[j + 2];
			if (chr == tmp)
			{
				index[c++] = j;
			}
		}
		if (c > 2)
		{
			for (int k = 1; k < c; k++)//计算距离
			{
				index[k] = index[k] - index[0];
			}
			int g = gcd(index[1], index[2]);
			for (int k = 3; k < c; k++)
			{
				g = gcd(g, index[k]);
			}
			if (m == 0)m = g;
			else {
				if (m == g)m = g;
				else m = gcd(m, g);
			}
		}
	}
	return m;
}
string Analysis(string s)
{
	s = str2lower(s);
	double mg;//明文指数
	double Ic;//重合指数
	Ic = Mg(s, true);
	printf("密文重合指数Ic=%f\n", Ic);
	fw = fopen("字母出现概率表.csv", "w");
	fprintf(fw, "字母,频数,频率\n");
	for (int i = 0; i < 26; i++)
	{
		fprintf(fw, "%c,%d,%.3f\n", (char)i + 'A', Fre[i], _rate[i]);
	}
	fclose(fw);
	if (Ic > 0.06)//Ic=0.065,说明不是维吉尼亚密码，是个单表代换
	{
		//先暴力穷举秘钥，判断是不是移位密码或者仿射密码，用明文指数来判断解密出来的明文是否有意义
		bool f = true;
		for (int i = 1; i < 26; i = i + 2)
		{
			if (i == 13)continue;
			for (int j = 1; j < 26; j++)
			{
				string m = Affine_Dncode(i, j, s);
				mg = Mg(m);
				if (mg > 0.06)
				{
					f = false;
					cout << i << " " << j << ":" << m << endl << endl;
				}
			}
		}
		if (f)
		{
			cout << "加密方式不是仿射密码或维吉尼亚密码" << endl;
		}

	}
	else //mg！=0.065
	{
		if ((fp = fopen("维吉尼亚分析表.csv", "w")) == NULL)
		{
			cout << "Error" << endl;
			
		}
		int m = Kasiski(s);
		fprintf(fp, "Kasiski计算的m值,%d\n", m);

		printf("Kasiski计算的m值,%d\n", m);
		if (m > 20)m = 0;
		double mmg = 0;
		fprintf(fp, "输出分组为m的各组重合指数\n");
		for (int i = 1; i <= 20; i++)
		{
			string tmp[20];
			for (int j = 0; j < s.length(); j++)
			{
				tmp[j % i] += s[j];
			}
			mmg = 0;
			fprintf(fp,"m=%d,", i);
			for (int j = 0; j < i; j++)
			{

				tmp[j][tmp[j].length()] = '\0';
				fprintf(fp,"%.4f,", Mg(tmp[j], true));
				mmg = mmg + Mg(tmp[j], true);
			}

			mmg = mmg / i;
			int n = 20 - i;
			while (n--)fprintf(fp, ",");
			fprintf(fp,"%.4f\n", mmg);
			if (m == i && mmg < 0.06)m = 0;
			if (mmg >= 0.06)
			{
				if (m == 0||m==1)m = i;
				else m = gcd(m, i);

			}
		}

		cout <<"m=" << m << endl;
		string tmp[20];
		for (int i = 0; i < s.length(); i++)
		{
			tmp[i % m] += s[i];
		}
		string tmpkey = "";
		string kkey[20];
		for (int i = 0; i < m; i++)
		{
			double mmgtmp = 100;//另一种评价mg的指标
			int jj = 0;
			string ttmp = tmp[i];
			for (int j = 0; j < 26; j++)
			{
				string mtmp = Affine_Dncode(1, j, ttmp);
				double mgtmp = Mg(mtmp);
				double a = 0.065 > mgtmp ? 0.065 - mgtmp : mgtmp - 0.065;
				if (a < mmgtmp) {
					jj = j;
					mmgtmp = a;
				}
				if (mgtmp > 0.06) {
					
					tmpkey += (char)(j + 'a');
					
					kkey[i] += (char)(j + 'a');
					//break;
				}
				fprintf(fp, "%.4f,", mgtmp);
			
			}
			if (i == tmpkey.length())
			{
				tmpkey += (char)(jj + 'a');
				kkey[i] += (char)(jj + 'a');
			}
			fprintf(fp, "\n");
		}
		fclose(fp);
		printf("key=");
		cout << tmpkey << endl;
		for (int i = 0; i < m; i++)
		{
			cout<<i<<" " << kkey[i] << endl;
		}
		fp= fopen("明文.txt", "w");

		fprintf(fp, "%s\n",tmpkey.c_str());
		//cout << tmpkey << endl;
		string M = "decode failed!";
		if (tmpkey.length()) {
			M = Vigenere_Dncode(s, tmpkey);
			if (Mg(M) < 0.06) {
				M = "解密出错";
			}
		}
		fprintf(fp, "%s\n", M.c_str());
		fclose(fp);	
		cout << M << endl;
		return M;
	}
	return s;
}
int main()
{

	string s = "BNVSNSIHQCEELSSKKYERIFJKXUMBGYKAMQLJTYAVFBKVTDVBPVVRJYYLAOKYMPQSCGDLFSRLLPROYGESEBUUALRWXMMASAZLGLEDFJBZAVVPXWICGJXASCBYEHOSNMULKCEAHTQOKMFLEBKFXLRREDTZXCIWBJSICBGAWDVYDHAVEJXZIBKCGJIWEAHTTOEWTUHKRQVVRGZBXYIREMMASCSPBHLHJMBLRFFJELHWEYLWISTFVVYEJCMHYUYRUFSFMGESIGRLWALSWMNUHSIMYYITCCQPZSICEHBCCMZFEGVJYOCDEMMPGHVAAUMELCMOEHVLTIPSUYILVGFLMVWDVYDBTHFRAYISYSGKVSUUHYHGGCKTMBLRX";
	cout << s.length() << endl;
	s=Analysis(s);
	//s = "ilearnedhowtocalculatetheamountofpaperneededforaroomwheniwasatschoolyoumultiplythesquarefootageofthewallsbythecubiccontentsofthefloorandceilingcombinedanddoubleityouthenallowhalfthetotalforopeningssuchaswindowsanddoorsthenyouallowtheotherhalfformatchingthepatternthenyoudoublethewholethingagaintogiveamarginoferrorandthenyouorderthepaper";

	//for (int i = 0; i < 26; i++)
	//{
	//	for (int j = 0; j < 26; j++)
	//	{
	//		for (int k = 0; k<26; k++)
	//		{
	//			string key;
	//			key = key + (char)(i + 'a') + (char)(j + 'a') + (char)(k + 'a');
	//			int m = Kasiski(Vigenere_Encode(s, key));
	//			//cout << m;
	//			if (m != 1 && m)
	//			{
	//				cout << key << "  " <<m<< endl;
	//			}
	//		}
	//		
	//	}
	//}

	//s = "igrewupamongslowtalkersmeninparticularwhodroppedwordsafewatatimelikebeansinahillandwhenigottominneapoliswherepeopletookalakewobegoncommatomeantheendnfastoryicouldntspeakawhnlesentenceincompanyandwasconsiderednottoobriahtsoienrolledinaspeechcouqsetaughtbyorvillesandthefounderofreflexiverelaxologyaselfhypnotictechniquethatenabledapersontospeakuptothreehundredwordsperminute";
	//s = s + s;//消息太短，获取信息量不够，容易出现解密出错的问题
	//s = "ilearnedhowtocalculatetheamountofpaperneededforaroomwheniwasatschoolyoumultiplythesquarefootageofthewallsbythecubiccontentsofthefloorandceilingcombinedanddoubleityouthenallowhalfthetotalforopeningssuchaswindowsanddoorsthenyouallowtheotherhalfformatchingthepatternthenyoudoublethewholethingagaintogiveamarginoferrorandthenyouorderthepaper";
	while (1)
	{
		int n=1;
		cout << "1表示仿射密码加密\n2表示维吉尼亚密码加密\n输入一个数（输入其他内容退出程序）：\n";
		cin >> n;
		if (n == 1)
		{
			int a, b;
			cout << "输入a,b" << endl;
			cin >> a >> b;
			string S = Affine_Encode(a, b, s);
			if (S == "Error")
			{
				cout << S << endl;
			}
			else {
				Analysis(S);
			}
			
		}
		else if(n==2){
			string key;
			cout << "输入加密密钥key:\n";
			cin >> key;
			string S = Vigenere_Encode(s, key);
			Analysis(S);
		}
		else
		{
			break;
		}
	}
	return 0;
}