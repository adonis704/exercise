#ifndef tree_h
#define tree_h
#include<stack>
#include <sstream> 
#include"heap.h"
#include"heap.cpp"
#include"treeNode.h"


class Tree {
private:
	TreeNode* root;	//���ڵ�
	string text;	//�������ı�
	double _averageLen;	//ƽ�����볤��
	vector<vector<string>> decodeTable;	//�����

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