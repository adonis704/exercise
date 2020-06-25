#include"tree.h"
#include<fstream>
using namespace std;
const int SIZE = 10000;

string toString(char ch) {
	stringstream ss;
	ss << ch;
	return ss.str();
}
//随机生成26个字母和空格的组合
string randomString() {
	string res = "";
	int temp;
	for (int i = 0; i < SIZE; i++) {
		temp = rand() % 27;
		if (temp == 26) temp = 32;
		else temp += 97;
		res = res + toString((char)temp);
	}
	return res;
}


int main() {
	//string randS;
	//randS = randomString();
	//ofstream raw("random.txt");
	//raw << randS;
	//raw.close();

	//从txt文档读取文本
	string s = "";
	string temp = "";
	ifstream in("artical.txt");
	while (in >> temp) {
		s = s + temp;
		s = s + " ";
	}
	in.close();

	//string s = "asdaA875120745412074 ";

	//编码解码等操作
	Tree tree(s);
	tree.build();
	string encode = tree.encode(s);
	string decode = tree.decode(encode);

	//将编码解码后的文本写入相应文档
	ofstream encodeS("encode.txt");
	ofstream decodeS("decode.txt");
	encodeS << encode;
	decodeS << decode;
	encodeS.close();
	decodeS.close();

	//输出信息
	tree.printTable();
	double rate = (double)encode.size() / ((double)s.size() * 5.0);
	if (s == decode) {
		cout << "correct!" << endl;
		cout << "average coding length:" << tree.averageLen() << endl;
		cout << "compress rate:" << 1.0 - tree.averageLen()/5.0 << endl;
	}
	system("pause");
}