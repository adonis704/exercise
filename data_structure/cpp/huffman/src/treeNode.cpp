#include"treeNode.h"

TreeNode::TreeNode(char d, int w, TreeNode* p , TreeNode* lc , TreeNode* rc) {
	data = d;
	weight = w;
	parent = p;
	lchild = lc;
	rchild = rc;
}
//判断是否是叶结点
bool TreeNode::isLeaf() {
	if (lchild == NULL && rchild == NULL) return true;
	return false;
}
//判断是否是父节点的左孩子
bool TreeNode::isLchild() {
	if (parent->lchild == this) return true;
	return false;
}
