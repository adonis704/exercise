#include"treeNode.h"

TreeNode::TreeNode(char d, int w, TreeNode* p , TreeNode* lc , TreeNode* rc) {
	data = d;
	weight = w;
	parent = p;
	lchild = lc;
	rchild = rc;
}
//�ж��Ƿ���Ҷ���
bool TreeNode::isLeaf() {
	if (lchild == NULL && rchild == NULL) return true;
	return false;
}
//�ж��Ƿ��Ǹ��ڵ������
bool TreeNode::isLchild() {
	if (parent->lchild == this) return true;
	return false;
}
