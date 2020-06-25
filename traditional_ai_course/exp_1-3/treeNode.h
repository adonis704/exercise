#pragma once
#include<vector>
#define NULL 0
using namespace std;
const int KIND_NUM = 3;	//����������
const double CONFIDENCE = 1.96;	//���Ŷȵ�ֵ

class TreeNode {
public:
	TreeNode* parent; TreeNode* leftChild; TreeNode* rightChild;
	int label, correct, wrong, total;	//��ǰ�����һ����࣬��ȷ������������һ���м���
	int properity;	//��ǰ�����ĸ����Էֿ�
	double bound;	//��չ����ֵ
	double error;	//����������
	vector<vector<double>> set;	//�������Щ����
	vector<int> expand;	//�ɶ��ֵ�����

	TreeNode(vector<vector<double>> s, vector<int> e, TreeNode* p = NULL, 
		TreeNode* lc = NULL, TreeNode* rc = NULL);
	TreeNode* insertAsLc(vector<vector<double>> s, vector<int> e);
	TreeNode* insertAsRc(vector<vector<double>> s, vector<int> e);
	bool isLeaf();
	void cal();
	void evalError(double num, double wrong, double conf);
};