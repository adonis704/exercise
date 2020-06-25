#include"treeNode.h"

//不可缺省的输入：数据，可扩展属性
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

//新建一个结点，包含数据与可拓展属性，作为左右结点插入
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

//根据结点包含的数据，计算相应的值
void TreeNode::cal() {
	vector<int> cnt;
	vector<double> temp;
	int setSize = set.size();
	for (int i = 0; i < KIND_NUM; i++)
		cnt.push_back(0);
	
	//统计结点数据中不同的类有多少
	for (vector<vector<double>>::iterator iter = set.begin(); iter != set.end(); iter++) {
		temp = *iter;
		cnt[temp[temp.size() - 1] - 1]++;
	}

	//统计结点数据中哪个类最多
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
//泛化误差估计
void TreeNode::evalError(double num, double wrong, double conf) {
	double rate = wrong / num;
	double res = (rate + conf * conf / (2 * num) + conf *
		sqrt(rate*(1 - rate) / num + conf * conf / (4 * num*num))) /
		(1 + conf * conf / num);
	error = res*num;
}

