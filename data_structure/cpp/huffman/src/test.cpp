#include"tree.h"
#include<fstream>
using namespace std;
const int SIZE = 10000;

string toString(char ch) {
	stringstream ss;
	ss << ch;
	return ss.str();
}
//�������26����ĸ�Ϳո�����
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

	//��txt�ĵ���ȡ�ı�
	string s = "";
	string temp = "";
	ifstream in("artical.txt");
	while (in >> temp) {
		s = s + temp;
		s = s + " ";
	}
	in.close();

	//string s = "asdaA875120745412074 ";

	//�������Ȳ���
	Tree tree(s);
	tree.build();
	string encode = tree.encode(s);
	string decode = tree.decode(encode);

	//������������ı�д����Ӧ�ĵ�
	ofstream encodeS("encode.txt");
	ofstream decodeS("decode.txt");
	encodeS << encode;
	decodeS << decode;
	encodeS.close();
	decodeS.close();

	//�����Ϣ
	tree.printTable();
	double rate = (double)encode.size() / ((double)s.size() * 5.0);
	if (s == decode) {
		cout << "correct!" << endl;
		cout << "average coding length:" << tree.averageLen() << endl;
		cout << "compress rate:" << 1.0 - tree.averageLen()/5.0 << endl;
	}
	system("pause");
}