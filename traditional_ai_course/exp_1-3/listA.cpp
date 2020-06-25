#include"listA.h"

ListA::ListA() {
	finish = over = false;
	int t[9] = { 1,2,3,8,0,4,7,6,5 };
	for (int i = 0; i < 9; i++)
		target.push_back(t[i]);
	cur = 0;
}

//拓展当前状态
void ListA::expand() {
	if (cur->state == target) {
		print(cur);
		finish = true;
		return;
	}
	move();	//当前状态不是目标状态，进行移动寻找新状态
	if (states.size() <= 1) over = true;	//open表为空，无解
	if (over) cout << "no solution.";
}
//以当前状态为基础寻找新状态
void ListA::move() {
	const int oprt[4] = { -3,3,-1,1 };	//上下左右
	vector<int> temp;
	temp = cur->state;
	int pos = -1; int posNew;
	//找到0在状态中的位置，后面移动的是0
	for (int i = 0; i < 9; i++)
		if (temp[i] == 0) {
			pos = i;
			break;
		}

	for (int i = 0; i < 4; i++) {
		//由于是一维数组，部分不合法状态不能通过越界判断，故用条件语句单独列出
		if (pos == 2 && i == RIGHT) continue;
		if (pos == 3 && i == LEFT) continue;
		if (pos == 5 && i == RIGHT) continue;
		if (pos == 6 && i == LEFT) continue;
		temp = cur->state;
		posNew = pos + oprt[i];

		//如果状态不越界且合法，交换0和相应数字的位置
		if (posNew >= 0 && posNew <= 8) {
			int tmp = temp[pos];
			temp[pos] = temp[posNew];
			temp[posNew] = tmp;
		}
		else continue;

		//如果状态在close表中存在，则继续下一次移动，否则建立一个新状态
		if (done(temp)) continue;
		StateA* possible = new StateA(temp, distance(temp), cur->deep + 1, cur);

		//确认新状态是否在open表中，如果在，则检查是否要更新，如果不在，则加入open表
		int old = find(temp);
		if (old != -1) exam(possible, old);
		else states.push(possible);
	}
}
//找到某个数在目标状态中的位置
int ListA::find(int num) {
	for (int i = 0; i < 9; i++)
		if (target[i] == num) return i;
	return -1;
}
//找到输入状态在优先队列中的位置
int ListA::find(vector<int> s) {
	//将优先队列中的状态全部放到数组中，便于操作
	vector<StateA*> temp;
	temp.push_back(cur);
	while (states.size() > 0) {
		temp.push_back(states.top());
		states.pop();
	}

	//找到输入状态在优先队列中的位置，没有则返回-1
	for (int i = 0; i < temp.size(); i++)
		if (temp[i]->state == s) {
			for (int j = 1; j < temp.size(); j++)
				states.push(temp[j]);
			return i;
		}
	if (temp.size() == 1) return -1;
	for (int i = 1; i < temp.size(); i++)
		states.push(temp[i]);
	return -1;
}
//计算状态与目标状态的距离
int ListA::distance(vector<int> s) {
	int res = 0;
	for (int i = 0; i < 9; i++) {
		if (s[i] == 0) continue;
		res += dist[i][find(s[i])];
	}
		
	return res;
}
//将状态加入优先队列
void ListA::add(StateA* x) {
	states.push(x);
}
//当前状态已经没有可以扩展的可能了，加入close表
StateA* ListA::dequeue() {
	cur = states.top();
	close.push_back(cur);
	states.pop();
	return cur;
}
//检查open表是否需要更新
void ListA::exam(StateA* s, int pos) {
	//将优先队列中的状态依次复制到数组中
	vector<StateA*> temp;
	temp.push_back(cur);
	while (states.size() > 0) {
		temp.push_back(states.top());
		states.pop();
	}

	//如果可能的状态在open表中，且代价小于open表的状态，则更新
	if (temp[pos]->dist + temp[pos]->deep > s->dist + s->deep)
	{
		//cout << "changing." << endl;
		temp[pos] = s;
	}

	for (int i = 1; i < temp.size(); i++)
		states.push(temp[i]);	
}
//检查状态是否在close表中
bool ListA::done(vector<int> s) {
	for (int i = 0; i < close.size(); i++)
		if (close[i]->state == s) return true;
	return false;
}

void ListA::init(string s) {
	vector<int> state;
	for (int i = 0; i < 9; i++)
		state.push_back((int)s[i] - 48);
	StateA* sa = new StateA(state, distance(state), 0, 0);
	states.push(sa);
}

bool ListA::isFinished() {
	return finish;
}
bool ListA::isOver() {
	return over;
}

void ListA::print(StateA* s) {
	stack<StateA*> temp;
	while (s->state != close[0]->state) {
		temp.push(s);
		s = s->parent;
	}
	cout << "original:" << endl;
	for (int i = 0; i < 9; i++) {
		string res;
		res = s->state[i] == 0 ? " " : to_string(s->state[i]);
		cout << res << " ";
		if (i == 2 || i == 5 || i == 8) cout << endl;
	}
	cout << "- - -  " << endl;
	int cnt = 1;
	while (temp.size() > 0) {
		cout << "Step " << cnt++ << ":" << endl;
		for (int i = 0; i < 9; i++) {
			string res;
			res = temp.top()->state[i] == 0 ? " " : to_string(temp.top()->state[i]);
			cout << res << " ";
			if (i == 2 || i == 5 || i == 8) cout << endl;
		}
		cout << "- - -  " << endl;
		temp.pop();
	}
}