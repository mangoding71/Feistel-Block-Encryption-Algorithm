#include<iostream>
#include<string>
#include <bitset>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

using namespace std;

/*定义及初始化S盒*/
const unsigned char S1[64] = {
		14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
        0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
        4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
        15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
	};

/*E是扩展置换，将4比特的R分组扩展为8比特，R=1234,E(R)=212343*/
string E(string str)
{
	string temp_string;
	temp_string += str[1];
	temp_string += str[0];
	temp_string += str[1];
	temp_string += str[2];
	temp_string += str[3];
	temp_string += str[2];
	return temp_string;
}

/*P盒置换,P(1234)=2413*/
string P(string str)
{
	string temp_string;
	temp_string += str[1];
	temp_string += str[3];
	temp_string += str[0];
	temp_string += str[2];
	return temp_string;
}

/*字符串和整型之间转换相关的函数调用*/
string initialization(string &str,int i,int bitset_num)
{
    int num;
    string str_1;
    num = (int)str[i];
    if(bitset_num==8)
    {
            bitset<8> bits(num);
            str_1 = bits.to_string();
    }
    if(bitset_num==4)
    {
            bitset<4> bits(num);
            str_1 = bits.to_string();
    }

	return str_1;
}


/*S盒变换*/
string S(string str)
{
	int num,row, col;
	row = 2 * (str[0] - '0') + (str[5] - '0');   /*第一位和第六位是表示行*/
	col = ((str[1] - '0') * 8) + ((str[2] - '0') * 4) + ((str[3] - '0') * 2) + (str[4] - '0');   /*第二三四五位表示列*/
	str = S1[row * 16 + col];  /*在S盒中取出相关的函数*/
	str=initialization(str,0,4);
	return str;
}

/*将明文分组为L与R*/
void makeGroup(string plaintext, string &L, string &R)
{
	L = "";
	R = "";/*L与R的初始化*/
	for (int i = 0; i < 4; i++)
		L += plaintext[i];
	for (int i = 4; i < 8; i++)
		R += plaintext[i];
}

/*异或函数，形参number是表示异或位数*/
string XOR(string str_1, string str_2,int number)
{

	string str;
	for (int i=0; i < number; i++)
		str+=((str_1[i] - '0') ^ (str_2[i] - '0') )+ '0';
	return str;

}

/*加密*/
void encryption(string &L,string &R,string key)
{
	string F_text;
	string temp=R;         /*保存R*/
	R = XOR(E(R), key,6);     /*E(R) ^ Ki,其中R = E(R)是将4位R拓展为6位*/
	F_text = P(S(R));    /*轮函数F，F=P(S ( E(R) ^ Ki))*/
	R = XOR(L, F_text,4);
	L = temp;       /*L=R*/
}

void exchange(string &L,string &R)
{
    string temp=R;
    R = L;
	L = temp;
}

void visual(bool flag)
{
    if(flag ==1)
    {
        cout << "**************************************************" << endl;
        cout << "   *********基于Feistel结构的分组加密**********   " << endl;
        cout << "         ************密码学**************        " << endl;
        cout << endl;
        cout << "******************* 基本参数 **********************"<<endl;
        cout << "**    四轮加密，分组长度为8比特，半个分组为4比特 **"<<endl;
        cout << "**    轮函数F=P(S ( E(R) ^ Ki))                  **"<<endl;
        cout << "**    K1=111111，K2=111000，K3=000111，K4=000000 **"<<endl;
        cout << "***************************************************"<<endl;
        cout << "请输入明文：";
    }
    else
        cout << "输出密文为：" << endl;


}

int main()
{
	string P,L,R;
	string plaintext;
	string key[4];

	visual(1);

	cin >> P;

    /*轮密钥编排从略，假定K1=111111，K2=111000，K3=000111，K4=000000*/
    key[0] = "111111";
	key[1] = "111000";
	key[2] = "000111";
	key[3] = "000000";

	visual(0);

	for (int i = 0; i < P.size();  i++)
	{
		plaintext = initialization(P,i,8);		/*将数值转化为字符串并返回对应的字符串*/
		makeGroup(plaintext, L, R);/*将字符串分为左右两部分*/

		for (int j = 0; j < 4; j++)/*进行4轮的加密*/
			encryption(L, R, key[j]);

        exchange(L,R);/*迭代完成后再将左右两部分交换位置，方便硬件实现*/

		cout << L << R ;
	}
	return 0;
}

