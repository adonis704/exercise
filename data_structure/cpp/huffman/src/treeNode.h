#ifndef treeNode_h
#define treeNode_h
#include<string>

#define NUll 0

using namespace std;
class TreeNode {
public:
	char data;	//字符
	int weight;	//权重
	TreeNode* parent;	//父结点
	TreeNode* lchild;	//左孩子
	TreeNode* rchild;	//右孩子

	TreeNode(char d, int w, TreeNode* p = NULL, TreeNode* lc = NULL, TreeNode* rc = NULL);
	bool isLeaf();
	bool isLchild();
};

#endif