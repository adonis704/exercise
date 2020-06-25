#ifndef tree_h
#define tree_h
#include<stack>
#include <sstream> 
#include"heap.h"
#include"heap.cpp"
#include"treeNode.h"


class Tree {
private:
	TreeNode* root;	//根节点
	string text;	//待编码文本
	double _averageLen;	//平均编码长度
	vector<vector<string>> decodeTable;	//解码表

	string toString(char ch);
	void buildTable();

public:
	Tree(string t);
	void build();
	void visit(TreeNode* node);
	void preOrder(TreeNode* node);
	void printTable();
	string encode(string t);
	string decode(string t);
	double averageLen();

};

#endif