#pragma once
#include<vector>
#define NULL 0
using namespace std;
const int KIND_NUM = 3;	//几分类问题
const double CONFIDENCE = 1.96;	//置信度的值

class TreeNode {
public:
	TreeNode* parent; TreeNode* leftChild; TreeNode* rightChild;
	int label, correct, wrong, total;	//当前结点哪一类最多，正确数，错误数，一共有几类
	int properity;	//当前结点从哪个属性分开
	double bound;	//扩展的阈值
	double error;	//泛化误差估计
	vector<vector<double>> set;	//结点有哪些数据
	vector<int> expand;	//可二分的属性

	TreeNode(vector<vector<double>> s, vector<int> e, TreeNode* p = NULL, 
		TreeNode* lc = NULL, TreeNode* rc = NULL);
	TreeNode* insertAsLc(vector<vector<double>> s, vector<int> e);
	TreeNode* insertAsRc(vector<vector<double>> s, vector<int> e);
	bool isLeaf();
	void cal();
	void evalError(double num, double wrong, double conf);
};