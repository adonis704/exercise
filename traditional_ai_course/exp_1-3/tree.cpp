#include"tree.h"

Tree::Tree(vector<vector<double>> s, int num) {
	vector<int> temp;
	temp.push_back(0);
	header = new TreeNode(s,temp);
	set = s;
	kindNum = num;
}

//��������Ϊ��������
TreeNode* Tree::insertAsRoot(vector<vector<double>> s) {
	int setSize = s[0].size() - 1;
	vector<int> expand;
	for (int i = 1; i <= setSize; i++)
		expand.push_back(i);
	TreeNode* temp = new TreeNode(s, expand,header);
	header->rightChild = temp;
	running.push(temp);
	root = temp;
	return temp;
}

//����n*2�����飬����һ���Լ���𣬷��ؿ��ܵĻ��ֵ�
vector<double> Tree::findDivide(vector<vector<double>> s) {
	int setSize = s.size();
	vector<vector<double>> sorted = sort(s);	//�����ݽ�������

	//�ҵ�������������ͬ���࣬�����ǵ�����ֵ�ľ�ֵ��Ϊ���ֵ�
	vector<double> res;
	for (int i = 0; i < setSize - 1; i++) {
		if (sorted[i][1] == sorted[i + 1][1]) continue;
		res.push_back((sorted[i][0] + sorted[i + 1][0]) / 2.0);
	}
	return res;
}
//�������ݣ���С����������򣬲���ð�����򷽷�������������
vector<vector<double>> Tree::sort(vector<vector<double>> s) {
	vector<vector<double>> res = s;
	vector<double> temp;
	int setSize = res.size();
	for (int i = 0; i < setSize - 1; i++)
		for (int j = 0; j < setSize - 1 - i; j++) {
			if (res[j][0] > res[j + 1][0]) {
				temp = res[j];
				res[j] = res[j + 1];
				res[j + 1] = temp;
			}
		}
	return res;
}

//����n*2���ݣ����ܵĻ��ֵ㣬������ѵĻ��ֵ����С��ֵ
vector<double> Tree::findBestPoint(vector<vector<double>> s, vector<double> divide) {
	vector<vector<double>> smaller;	//�Ȼ��ֵ�С������
	vector<vector<double>> bigger;	//�Ȼ��ֵ�������
	vector<double> res;
	double best = 0.0;
	double entropy = 10000.0;
	double testPoint, testEntropy;	//���ԵĻ��ֵ㣬��
	double setSize = s.size();
	double smallerSize, biggerSize;

	//��һ���Կ��ܵĻ��ֵ�
	for (int i = 0; i < divide.size(); i++) {
		//�Ի��ֵ�Ϊ�磬�����ݷ�Ϊ���뻮�ֵ��С�뻮�ֵ�������
		testPoint = divide[i];
		for (int i = 0; i < setSize; i++) {
			if (s[i][0] < testPoint) smaller.push_back(s[i]);
			else bigger.push_back(s[i]);
		}

		//ѡ����С��ֵ�Ļ��ֵ���Ϊ��ѻ��ֵ�
		smallerSize = smaller.size();
		biggerSize = bigger.size();
		testEntropy = smallerSize / setSize * cal(smaller) + biggerSize / setSize * cal(bigger);
		if (testEntropy < entropy) {
			entropy = testEntropy;
			best = testPoint;
		}
	}
	res.push_back(best);
	res.push_back(entropy);
	return res;
}
//��������n*2���ݣ�������ֵ
double Tree::cal(vector<vector<double>> s) {
	double res = 0.0;
	vector<double> cnt;
	double total = s.size();
	for (int i = 0; i < kindNum; i++)
		cnt.push_back(0.0);

	for (int i = 0; i < total; i++)
		cnt[s[i][1] - 1]++;
	for (int i = 0; i < kindNum; i++)
		if (cnt[i] != 0) res += -(cnt[i] / total)*log(cnt[i] / total);
	return res;
}

//��չ������
void Tree::expand() {
	//ѡһ���ɶ��ֽ��
	TreeNode* temp = running.front();	
	cout << "expand available:";
	for (int i = 0; i < temp->expand.size(); i++)
		cout << temp->expand[i];
	running.pop();

	int propBest = -1;
	int propTemp = -1;
	double entropyBest = 10000;
	double entropyTemp;
	double divideBest;
	vector<double> divideTemp;
	vector<vector<double>> setTemp;

	//��һ�����Զ��ֵ�����
	for (int i = 0; i < temp->expand.size(); i++) {
		propTemp = temp->expand[i];
		setTemp = slim(temp->set, propTemp-1);
		divideTemp = findBestPoint(setTemp, findDivide(setTemp));
		entropyTemp = divideTemp[1];

		//�Ƚ��������Ե���ֵ��ѡ����С��ֵ�����Զ���
		if (entropyTemp < entropyBest) {
			entropyBest = entropyTemp;
			propBest = propTemp;
			divideBest = divideTemp[0];
		}
	}
	temp->properity = propBest;
	temp->bound = divideBest;
	cout << ". expand properity" << propBest << ". ";
	cout << "bound:" << temp->bound << endl;

	//�Խ����ж���
	split(temp, propBest, divideBest);

}
//�����㡢�������ԡ����ֵ㣬�Խ����ж���
void Tree::split(TreeNode* cur, int properity, double divide) {
	vector<vector<double>> leftSet, rightSet;	//�������ݣ��ҽ������
	vector<int> expandNew;	//�ӽ��Ŀ���չ����
	for (int i = 0; i < cur->expand.size(); i++)
		if (cur->expand[i] != properity) expandNew.push_back(cur->expand[i]);

	//�Ի��ֵ�Ϊ�磬��Ϊ�������ݣ��ҽ������
	vector<double> temp;
	for (vector<vector<double>>::iterator iter = cur->set.begin(); iter != cur->set.end(); iter++) {
		temp = *iter;
		if (temp[properity-1] < divide) leftSet.push_back(temp);
		else rightSet.push_back(temp);
	}
	//����Ϊ���ҽ��
	TreeNode* left = cur->insertAsLc(leftSet, expandNew);
	TreeNode* right = cur->insertAsRc(rightSet, expandNew);

	////�÷��������Ϊǰ��֦�ı�׼
	//if (cur->error < cur->leftChild->error + cur->rightChild->error) {
	//	cutPre(cur);
	//	return;
	//}

	//�ñ��������Ϊǰ��֦�ı�׼
	double pessError = pess(cur->wrong + cur->correct, 1, cur->wrong, PUNISH);
	double pessErrorDivided = pess(cur->wrong + cur->correct, 3, cur->leftChild->wrong + cur->rightChild->wrong, PUNISH);
	if (pessError <= pessErrorDivided) {
		cutPre(cur);
		return;
	}

	//��㲻��Ҷ��㣬�ͼ���ɶ��ֶ���
	if (!checkLeaf(left)) running.push(left);
	else 	cout << "find left leaf when expand properity " << cur->properity << endl;
	if (!checkLeaf(right)) running.push(right);
	else 	cout << "find right leaf when expand properity " << cur->properity << endl;
}
//����Ƿ�ΪҶ���
bool Tree::checkLeaf(TreeNode* node) {
	//ֻ��һ�࣬���߲����ٶ��֣���ΪҶ�ڵ�
	if (node->total == 1 || node->expand.size()==1) return true;
	return false;
}
//���㱯�����
double Tree::pess(double total, double leaf, double wrong, double punish) {
	double res = (wrong + leaf * punish) / total;
	return res;
}
//����m*n���顢���������ԣ�����m*2�����飬һ��Ϊ�������ԣ�һ��Ϊ���
vector<vector<double>> Tree::slim(vector<vector<double>> s, int pos) {
	vector<vector<double>> res;
	vector<double> temp;
	vector<double> tempIter;
	for (vector<vector<double>>::iterator iter = s.begin(); iter != s.end(); iter++) {
		tempIter = *iter;
		temp.push_back(tempIter[pos]);
		temp.push_back(tempIter[tempIter.size()-1]);
		res.push_back(temp);
		temp.clear();
	}
	return res;
}
//����m*n���顢��Ҫȥ�������ԣ�����m*(n-1)�����飬ɾȥ��һ������
vector<vector<double>> Tree::sub(vector<vector<double>> s, int pos) {
	vector<vector<double>> res;
	vector<double> temp;
	for (vector<vector<double>>::iterator iter = s.begin(); iter != s.end(); iter++) {
		for(int i = 0; i < (*iter).size();i++)
			if (i != pos)	temp.push_back((*iter)[i]);
		res.push_back(temp);
		temp.clear();
	}
	return res;
}

//ѵ��������
void Tree::process() {
	insertAsRoot(set);
	while (running.size() > 0)
		expand();
	preOrder(root);
}

//�����㣬���м�֦
void Tree::cutPre(TreeNode* r) {
	delete r->leftChild;
	delete r->rightChild;
	r->bound = -1;
	r->properity = -1;
	cout << "pruning." << endl;
}
//�����������
void Tree::preOrder(TreeNode* r) {
	if (!r->isLeaf()) {
		preOrder(r->leftChild);
		preOrder(r->rightChild);
	}
	else {
		for (int i = 0; i < r->set.size(); i++)
			cout << r->set[i][r->set[i].size() - 1] << " ";
		cout << endl;
	}
}

void Tree::test(vector<vector<double>> s) {
	TreeNode* cur;
	double correct = 0, wrong = 0;
	for (int i = 0; i < s.size(); i++) {
		TreeNode* cur = root;
		while (!cur->isLeaf()) {
			int pos = cur->properity - 1;
			if (s[i][pos] < cur->bound) cur = cur->leftChild;
			else cur = cur->rightChild;
		}
		if (cur->label != s[i][s[i].size() - 1]) wrong++;
		else correct++;
	}
	double rate = correct / (correct + wrong);
	cout << "correct rate:" << rate << endl;
}


