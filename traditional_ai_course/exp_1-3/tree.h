#pragma once
#include<cmath>
#include<queue>
#include<iostream>
#include"treeNode.h"
const double PUNISH = 0.5;	//罚项
class Tree {
private:
	TreeNode* header; TreeNode* root;	//头结点，根结点（即由训练数据创建的结点）
	vector<vector<double>> set;	//训练数据
	queue<TreeNode*> running;	//待判断是否需要二分的结点
	int kindNum;	//几分类问题

public:
	Tree(vector<vector<double>> s, int num);
	vector<double> findDivide(vector<vector<double>> s);
	vector<double> findBestPoint(vector<vector<double>> s, vector<double> divide);
	void expand();
	void split(TreeNode* cur, int prop, double divide);
	double cal(vector<vector<double>> s);
	TreeNode* insertAsRoot(vector<vector<double>> s);
	vector<vector<double>> sort(vector<vector<double>> s);
	vector<vector<double>> slim(vector<vector<double>> s, int pos);
	vector<vector<double>> sub(vector<vector<double>> s, int pos);
	bool checkLeaf(TreeNode* node);
	void process();
	void preOrder(TreeNode* r);
	void cutPre(TreeNode* r);
	void test(vector<vector<double>> s);
	double pess(double total, double leaf, double wrong, double punish);
};