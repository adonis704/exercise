#include"tree.h"

Tree::Tree(string t) {
	text = t;
	_averageLen = 0.0;
}

//����huffman��
void Tree::build() {
	Heap<TreeNode*> heap;	//С����
	TreeNode* temp;

	//ͳ���ַ�Ƶ��
	vector<int> count;
	int pos;
	for (int i = 0; i < 27; i++)
		count.push_back(0);
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == ' ') count[26]++;
		else {
			if((int)text[i] - 97 >=0 && (int)text[i] - 97 < 27)
				count[(int)text[i] - 97]++;
		}
	}

	//ÿ���ַ���Ϊһ����㣬����
	for (int i = 0; i < count.size()-1; i++) {
		if (count[i] == 0) continue;
		temp = new TreeNode((char)(i + 97), count[i]);
		heap.insert(temp);
	}
	temp = new TreeNode(' ', count[26]);
	heap.insert(temp);

	//��huffman��
	TreeNode* first;
	TreeNode* second;
	while (heap.size() > 1) {
		//����ȡ������Ȩ����С�Ľ��
		first = heap.pop();
		second = heap.pop();
		//�ϲ������
		temp = new TreeNode('1', first->weight + second->weight,NULL,first,second);
		temp->lchild->parent = temp;
		temp->rchild->parent = temp;
		//���½����뵽����
		heap.insert(temp);
		//cout << "handle " << first->data << " and " << second->data << endl;
	}
	//����㼴Ϊ���ϲ�ʣ�µĽ��
	root = heap.front();
	buildTable();
}
//����
void Tree::buildTable() {
	preOrder(root);
}
//�������
void Tree::preOrder(TreeNode* node) {
	if (node == NULL) return;
	if (node->isLeaf()) {
		visit(node);
		return;
	}
	preOrder(node->lchild);
	preOrder(node->rchild);
}
//��Ҷ������
void Tree::visit(TreeNode* node) {
	double weight = node->weight;
	string ch = toString(node->data);
	string code = "";
	
	//��Ҷ�����ݣ������������ô����0��������Һ�����ô����1
	while (node != root) {
		if (node->isLchild()) code += "0";
		else code += "1";
		node = node->parent;
	}
	//����˳�������˳�������෴���ʷ�ת
	string codeReverse = "";
	for (int i = code.size()-1; i >= 0; i--)
		codeReverse = codeReverse + toString(code[i]);
	//Ҷ���ı��볤��*Ȩ��
	_averageLen += (double)codeReverse.size()*(weight / (double)text.size());

	//���뵽�������
	vector<string> temp;
	temp.push_back(ch);
	temp.push_back(codeReverse);
	decodeTable.push_back(temp);
}
//�����ַ�������ַ���
string Tree::toString(char ch) {
	stringstream stream;
	stream << ch;
	string str = stream.str();
	return str;
}

//�����ַ���������������ַ���
string Tree::encode(string t) {
	string res = "";
	//�����ַ����е�ÿһ���ַ����ڱ�������ҵ���Ӧ�ַ��ı��뼴��
	for (int i = 0; i < t.size(); i++) 
		for (int j = 0; j < decodeTable.size(); j++) 
			if (decodeTable[j][0] == toString(t[i])) {
				res = res + decodeTable[j][1];
				break;
			}
	return res;
}

//�����ַ������������õ����ַ���
string Tree::decode(string t) {
	TreeNode* cur = NULL;
	string res = "";

	for (int i = 0; i < t.size();) {
		cur = root;
		//�Ӹ��ڵ㿪ʼ�������0�������ߣ���������������ߣ�ֱ������Ҷ��㣬���һ���ַ��Ľ���
		while (!cur->isLeaf()) {
			if (t[i] == '0') cur = cur->lchild;
			else cur = cur->rchild;
			i++;
		}
		res = res + toString(cur->data);
		//cout << "res: " << res << endl;
	}
	return res;
}

//��ӡ�����
void Tree::printTable() {
	cout << "- - - - - - - - - - - -" << endl;
	cout << "Decode table:" << endl;
	for (int i = 0; i < decodeTable.size(); i++) {
		cout << "char:" << decodeTable[i][0] << "  code:" << decodeTable[i][1] << endl;
	}
	cout << "- - - - - - - - - - - -" << endl;
}

double Tree::averageLen() {
	return _averageLen;
}