#include"treeNode.h"

//����ȱʡ�����룺���ݣ�����չ����
TreeNode::TreeNode(vector<vector<double>> s, vector<int> e, TreeNode* p , TreeNode* lc , TreeNode* rc) {
	set = s; expand = e;
	parent = p; leftChild = lc; rightChild = rc;
	properity = -1;
	bound = -1;
	label = -1;
	correct = 0;
	wrong = 0;
	cal();
}

//�½�һ����㣬�������������չ���ԣ���Ϊ���ҽ�����
TreeNode* TreeNode::insertAsLc(vector<vector<double>> s, vector<int> e) {
	TreeNode* temp = new TreeNode(s, e, this);
	leftChild = temp;
	return temp;
}
TreeNode* TreeNode::insertAsRc(vector<vector<double>> s, vector<int> e) {
	TreeNode* temp = new TreeNode(s, e, this);
	rightChild = temp;
	return temp;
}

bool TreeNode::isLeaf() {
	if (bound == -1) return true;
	return false;
}

//���ݽ����������ݣ�������Ӧ��ֵ
void TreeNode::cal() {
	vector<int> cnt;
	vector<double> temp;
	int setSize = set.size();
	for (int i = 0; i < KIND_NUM; i++)
		cnt.push_back(0);
	
	//ͳ�ƽ�������в�ͬ�����ж���
	for (vector<vector<double>>::iterator iter = set.begin(); iter != set.end(); iter++) {
		temp = *iter;
		cnt[temp[temp.size() - 1] - 1]++;
	}

	//ͳ�ƽ���������ĸ������
	int maxNum = -1;
	int maxKind = -1;
	int totalKind = 0;
	for (int i = 0; i < cnt.size(); i++) {
		if (maxNum < cnt[i]) {
			maxNum = cnt[i];
			maxKind = i + 1;
		}
		if (cnt[i] > 0) totalKind++;
	}
	total = totalKind;
	label = maxKind;
	correct = cnt[maxKind - 1];
	wrong = set.size() - correct;
	evalError(set.size(), wrong, CONFIDENCE);
}
//����������
void TreeNode::evalError(double num, double wrong, double conf) {
	double rate = wrong / num;
	double res = (rate + conf * conf / (2 * num) + conf *
		sqrt(rate*(1 - rate) / num + conf * conf / (4 * num*num))) /
		(1 + conf * conf / num);
	error = res*num;
}

