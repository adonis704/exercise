#include"tree.h"

Tree::Tree(string t) {
	text = t;
	_averageLen = 0.0;
}

//建立huffman树
void Tree::build() {
	Heap<TreeNode*> heap;	//小顶堆
	TreeNode* temp;

	//统计字符频率
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

	//每个字符作为一个结点，建堆
	for (int i = 0; i < count.size()-1; i++) {
		if (count[i] == 0) continue;
		temp = new TreeNode((char)(i + 97), count[i]);
		heap.insert(temp);
	}
	temp = new TreeNode(' ', count[26]);
	heap.insert(temp);

	//建huffman树
	TreeNode* first;
	TreeNode* second;
	while (heap.size() > 1) {
		//两次取出堆中权重最小的结点
		first = heap.pop();
		second = heap.pop();
		//合并两结点
		temp = new TreeNode('1', first->weight + second->weight,NULL,first,second);
		temp->lchild->parent = temp;
		temp->rchild->parent = temp;
		//将新结点加入到堆中
		heap.insert(temp);
		//cout << "handle " << first->data << " and " << second->data << endl;
	}
	//根结点即为最后合并剩下的结点
	root = heap.front();
	buildTable();
}
//建表
void Tree::buildTable() {
	preOrder(root);
}
//先序遍历
void Tree::preOrder(TreeNode* node) {
	if (node == NULL) return;
	if (node->isLeaf()) {
		visit(node);
		return;
	}
	preOrder(node->lchild);
	preOrder(node->rchild);
}
//给叶结点编码
void Tree::visit(TreeNode* node) {
	double weight = node->weight;
	string ch = toString(node->data);
	string code = "";
	
	//从叶结点回溯，如果是左孩子那么就是0，如果是右孩子那么就是1
	while (node != root) {
		if (node->isLchild()) code += "0";
		else code += "1";
		node = node->parent;
	}
	//回溯顺序与编码顺序正好相反，故反转
	string codeReverse = "";
	for (int i = code.size()-1; i >= 0; i--)
		codeReverse = codeReverse + toString(code[i]);
	//叶结点的编码长度*权重
	_averageLen += (double)codeReverse.size()*(weight / (double)text.size());

	//加入到解码表中
	vector<string> temp;
	temp.push_back(ch);
	temp.push_back(codeReverse);
	decodeTable.push_back(temp);
}
//输入字符，输出字符串
string Tree::toString(char ch) {
	stringstream stream;
	stream << ch;
	string str = stream.str();
	return str;
}

//输入字符串，输出编码后的字符串
string Tree::encode(string t) {
	string res = "";
	//遍历字符串中的每一个字符，在编码表中找到相应字符的编码即可
	for (int i = 0; i < t.size(); i++) 
		for (int j = 0; j < decodeTable.size(); j++) 
			if (decodeTable[j][0] == toString(t[i])) {
				res = res + decodeTable[j][1];
				break;
			}
	return res;
}

//输入字符串，输出解码得到的字符串
string Tree::decode(string t) {
	TreeNode* cur = NULL;
	string res = "";

	for (int i = 0; i < t.size();) {
		cur = root;
		//从根节点开始，如果是0则往左走，如果是右则往右走，直至遇到叶结点，完成一个字符的解码
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

//打印解码表
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