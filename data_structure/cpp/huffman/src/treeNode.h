#ifndef treeNode_h
#define treeNode_h
#include<string>

#define NUll 0

using namespace std;
class TreeNode {
public:
	char data;	//�ַ�
	int weight;	//Ȩ��
	TreeNode* parent;	//�����
	TreeNode* lchild;	//����
	TreeNode* rchild;	//�Һ���

	TreeNode(char d, int w, TreeNode* p = NULL, TreeNode* lc = NULL, TreeNode* rc = NULL);
	bool isLeaf();
	bool isLchild();
};

#endif