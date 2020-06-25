#include"function.h"

Function::Function(int mMax, int cMax, int nMax) {
	finish = false; over = false;
	header = new State();
	root = new State(mMax, cMax, 1, header, 0);
	running.push(root);
	vector<int> row;
	possible.push_back("NULL");
	if (mMax > MEMBER_MAX) mMax = MEMBER_MAX;
	if (cMax > MEMBER_MAX) cMax = MEMBER_MAX;
	if (nMax > MEMBER_MAX) nMax = MEMBER_MAX;

	//生成可能的状态，可能的操作
	for(int i = 0; i<=mMax;i++)
		for (int j = 0; j <= cMax; j++) {
			if (i + j <= nMax && i + j > 0) {
				row.push_back(i);
				row.push_back(j);
				oprt.push_back(row);
				row.resize(0);
				//cout << "add operator" << i << " " << j << endl;
			}
			int tempM = mMax - i, tempC = cMax - j;
			if (i == mMax && j == cMax) break;
			if ((i == 0 || i >= j) && (tempM == 0 || tempM >= tempC) ) {
				possible.push_back(to_string(i) + to_string(j) + "0");
				possible.push_back(to_string(i) + to_string(j) + "1");
				//cout << "add possible state" << i << " " << j << endl;
			}
		}

}

Function::~Function(){}

//扩展状态
void Function::expand() {
	State* cur = 0;
	//选择一个可扩展状态
	if (running.size() > 0) {
		cur = running.front();
		running.pop();
	}
	else {
		over = true;
		print(cur);
		return;
	}

	//根据状态中的n判断船在哪边，实行不同操作
	switch (cur->n) {
	case 0:
		rightToLeft(cur);
		break;
	case 1:
		cur = leftToRight(cur);
		break;
	}

	if (finish) print(cur);
}

//从左移到右
State* Function::leftToRight(State* cur) {
	int move1, move2,res1,res2;
	State* res = 0;

	for (int i = 0; i < oprt.size(); i++) {	//检查所有可能操作
		move1 = oprt[i][0];
		move2 = oprt[i][1];
		res1 = cur->m - move1;	//可能的新状态
		res2 = cur->c - move2;
		vector<string>::iterator iter = check(res1, res2, 0);
		if ( *iter != "NULL" ) {
			res = cur->setChildren(res1, res2, 0);	//将新状态设置为当前状态的子状态
			//cout << "trying move " << move1 << " chuanjiaoshi and " << move2 << " yeren to right." << endl;
			running.push(res);
			possible.erase(iter);	//如果状态合法，则将该状态放入running后，从可能状态移除
			if (res1 == 0 && res2 == 0) {
				finish = true;	//达到目标状态
				return res;
			}
		}
	}
	return res;
}

void Function::rightToLeft(State* cur) {
	int move1, move2, res1, res2;
	for (int i = 0; i  < oprt.size() ; i++) {
		move1 = oprt[i][0];
		move2 = oprt[i][1];
		res1 = cur->m + move1;
		res2 = cur->c + move2;
		vector<string>::iterator iter = check(res1, res2, 1);
		if (*iter != "NULL") {
			//cout << "trying move " << move1 << " chuanjiaoshi and " << move2 << " yeren to left." << endl;
			running.push(cur->setChildren(res1, res2, 1));
			possible.erase(iter);
		}
	}
}

//检查状态是否合法
vector<string>::iterator Function::check(int ms, int cs, int ns) {
	string temp = to_string(ms) + to_string(cs) + to_string(ns);
	for (vector<string>::iterator iter = possible.begin(); iter != possible.end(); iter++)
		if (*iter == temp) return iter;
	vector<string>::iterator iter = possible.begin();
	return iter;
}

bool Function::isFinished(){
	return finish;
}

bool Function::isOver() {
	return over;
}

void Function::print(State* cur) {
	if (finish) {
		//从目标状态开始，通过指向父状态的指针，将通路加入到栈中
		stack<State*> path;
		while (cur != root) {
			path.push(cur);
			cur = cur->parent;
		}
		path.push(root);

		//打印操作过程
		while (path.size() > 0) {
			State* temp = path.top();
			path.pop();
			if (temp != root) {
				int move1, move2;
				move1 = abs(temp->m - temp->parent->m);
				move2 = abs(temp->c - temp->parent->c);
				cout << "Move " << move1 << " missionaries and " << move2 << " cannibals to";
				if (temp->n == 0) cout << " right coast." << endl;
				else cout << " left coast." << endl;
			}
			if (temp == root)
				cout << "Original state: ";
			else cout << "State: ";
			cout << "("<< temp->m << ", " << temp->c << ", " << temp->n << ")" << endl << endl;
		}
	}
	else cout << "No solution." << endl;
}
