#include"tree.h"

Tree::Tree(vector<vector<double>> s, int num) {
	vector<int> temp;
	temp.push_back(0);
	header = new TreeNode(s,temp);
	set = s;
	kindNum = num;
}

//将数据作为根结点插入
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

//输入n*2的数组，即单一属性加类别，返回可能的划分点
vector<double> Tree::findDivide(vector<vector<double>> s) {
	int setSize = s.size();
	vector<vector<double>> sorted = sort(s);	//对数据进行排序

	//找到连续的两个不同的类，将他们的属性值的均值作为划分点
	vector<double> res;
	for (int i = 0; i < setSize - 1; i++) {
		if (sorted[i][1] == sorted[i + 1][1]) continue;
		res.push_back((sorted[i][0] + sorted[i + 1][0]) / 2.0);
	}
	return res;
}
//输入数据，由小到大进行排序，采用冒泡排序方法，返回排序结果
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

//输入n*2数据，可能的划分点，返回最佳的划分点和最小熵值
vector<double> Tree::findBestPoint(vector<vector<double>> s, vector<double> divide) {
	vector<vector<double>> smaller;	//比划分点小的数据
	vector<vector<double>> bigger;	//比划分点大的数据
	vector<double> res;
	double best = 0.0;
	double entropy = 10000.0;
	double testPoint, testEntropy;	//测试的划分点，熵
	double setSize = s.size();
	double smallerSize, biggerSize;

	//逐一测试可能的划分点
	for (int i = 0; i < divide.size(); i++) {
		//以划分点为界，将数据分为大与划分点和小与划分点两部分
		testPoint = divide[i];
		for (int i = 0; i < setSize; i++) {
			if (s[i][0] < testPoint) smaller.push_back(s[i]);
			else bigger.push_back(s[i]);
		}

		//选择最小熵值的划分点作为最佳划分点
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
//输入数据n*2数据，返回熵值
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

//拓展决策树
void Tree::expand() {
	//选一个可二分结点
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

	//逐一检查可以二分的属性
	for (int i = 0; i < temp->expand.size(); i++) {
		propTemp = temp->expand[i];
		setTemp = slim(temp->set, propTemp-1);
		divideTemp = findBestPoint(setTemp, findDivide(setTemp));
		entropyTemp = divideTemp[1];

		//比较所有属性的熵值，选择最小熵值的属性二分
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

	//对结点进行二分
	split(temp, propBest, divideBest);

}
//输入结点、二分属性、划分点，对结点进行二分
void Tree::split(TreeNode* cur, int properity, double divide) {
	vector<vector<double>> leftSet, rightSet;	//左结点数据，右结点数据
	vector<int> expandNew;	//子结点的可扩展属性
	for (int i = 0; i < cur->expand.size(); i++)
		if (cur->expand[i] != properity) expandNew.push_back(cur->expand[i]);

	//以划分点为界，分为左结点数据，右结点数据
	vector<double> temp;
	for (vector<vector<double>>::iterator iter = cur->set.begin(); iter != cur->set.end(); iter++) {
		temp = *iter;
		if (temp[properity-1] < divide) leftSet.push_back(temp);
		else rightSet.push_back(temp);
	}
	//插入为左右结点
	TreeNode* left = cur->insertAsLc(leftSet, expandNew);
	TreeNode* right = cur->insertAsRc(rightSet, expandNew);

	////用泛化误差作为前剪枝的标准
	//if (cur->error < cur->leftChild->error + cur->rightChild->error) {
	//	cutPre(cur);
	//	return;
	//}

	//用悲观误差作为前剪枝的标准
	double pessError = pess(cur->wrong + cur->correct, 1, cur->wrong, PUNISH);
	double pessErrorDivided = pess(cur->wrong + cur->correct, 3, cur->leftChild->wrong + cur->rightChild->wrong, PUNISH);
	if (pessError <= pessErrorDivided) {
		cutPre(cur);
		return;
	}

	//结点不是叶结点，就加入可二分队列
	if (!checkLeaf(left)) running.push(left);
	else 	cout << "find left leaf when expand properity " << cur->properity << endl;
	if (!checkLeaf(right)) running.push(right);
	else 	cout << "find right leaf when expand properity " << cur->properity << endl;
}
//检查是否为叶结点
bool Tree::checkLeaf(TreeNode* node) {
	//只有一类，或者不能再二分，即为叶节点
	if (node->total == 1 || node->expand.size()==1) return true;
	return false;
}
//计算悲观误差
double Tree::pess(double total, double leaf, double wrong, double punish) {
	double res = (wrong + leaf * punish) / total;
	return res;
}
//输入m*n数组、保留的属性，返回m*2的数组，一列为保留属性，一列为类别
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
//输入m*n数组、需要去除的属性，返回m*(n-1)的数组，删去了一列属性
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

//训练决策树
void Tree::process() {
	insertAsRoot(set);
	while (running.size() > 0)
		expand();
	preOrder(root);
}

//输入结点，进行剪枝
void Tree::cutPre(TreeNode* r) {
	delete r->leftChild;
	delete r->rightChild;
	r->bound = -1;
	r->properity = -1;
	cout << "pruning." << endl;
}
//树的先序遍历
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


