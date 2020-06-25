#include"listA.h"

ListA::ListA() {
	finish = over = false;
	int t[9] = { 1,2,3,8,0,4,7,6,5 };
	for (int i = 0; i < 9; i++)
		target.push_back(t[i]);
	cur = 0;
}

//��չ��ǰ״̬
void ListA::expand() {
	if (cur->state == target) {
		print(cur);
		finish = true;
		return;
	}
	move();	//��ǰ״̬����Ŀ��״̬�������ƶ�Ѱ����״̬
	if (states.size() <= 1) over = true;	//open��Ϊ�գ��޽�
	if (over) cout << "no solution.";
}
//�Ե�ǰ״̬Ϊ����Ѱ����״̬
void ListA::move() {
	const int oprt[4] = { -3,3,-1,1 };	//��������
	vector<int> temp;
	temp = cur->state;
	int pos = -1; int posNew;
	//�ҵ�0��״̬�е�λ�ã������ƶ�����0
	for (int i = 0; i < 9; i++)
		if (temp[i] == 0) {
			pos = i;
			break;
		}

	for (int i = 0; i < 4; i++) {
		//������һά���飬���ֲ��Ϸ�״̬����ͨ��Խ���жϣ�����������䵥���г�
		if (pos == 2 && i == RIGHT) continue;
		if (pos == 3 && i == LEFT) continue;
		if (pos == 5 && i == RIGHT) continue;
		if (pos == 6 && i == LEFT) continue;
		temp = cur->state;
		posNew = pos + oprt[i];

		//���״̬��Խ���ҺϷ�������0����Ӧ���ֵ�λ��
		if (posNew >= 0 && posNew <= 8) {
			int tmp = temp[pos];
			temp[pos] = temp[posNew];
			temp[posNew] = tmp;
		}
		else continue;

		//���״̬��close���д��ڣ��������һ���ƶ���������һ����״̬
		if (done(temp)) continue;
		StateA* possible = new StateA(temp, distance(temp), cur->deep + 1, cur);

		//ȷ����״̬�Ƿ���open���У�����ڣ������Ƿ�Ҫ���£�������ڣ������open��
		int old = find(temp);
		if (old != -1) exam(possible, old);
		else states.push(possible);
	}
}
//�ҵ�ĳ������Ŀ��״̬�е�λ��
int ListA::find(int num) {
	for (int i = 0; i < 9; i++)
		if (target[i] == num) return i;
	return -1;
}
//�ҵ�����״̬�����ȶ����е�λ��
int ListA::find(vector<int> s) {
	//�����ȶ����е�״̬ȫ���ŵ������У����ڲ���
	vector<StateA*> temp;
	temp.push_back(cur);
	while (states.size() > 0) {
		temp.push_back(states.top());
		states.pop();
	}

	//�ҵ�����״̬�����ȶ����е�λ�ã�û���򷵻�-1
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
//����״̬��Ŀ��״̬�ľ���
int ListA::distance(vector<int> s) {
	int res = 0;
	for (int i = 0; i < 9; i++) {
		if (s[i] == 0) continue;
		res += dist[i][find(s[i])];
	}
		
	return res;
}
//��״̬�������ȶ���
void ListA::add(StateA* x) {
	states.push(x);
}
//��ǰ״̬�Ѿ�û�п�����չ�Ŀ����ˣ�����close��
StateA* ListA::dequeue() {
	cur = states.top();
	close.push_back(cur);
	states.pop();
	return cur;
}
//���open���Ƿ���Ҫ����
void ListA::exam(StateA* s, int pos) {
	//�����ȶ����е�״̬���θ��Ƶ�������
	vector<StateA*> temp;
	temp.push_back(cur);
	while (states.size() > 0) {
		temp.push_back(states.top());
		states.pop();
	}

	//������ܵ�״̬��open���У��Ҵ���С��open���״̬�������
	if (temp[pos]->dist + temp[pos]->deep > s->dist + s->deep)
	{
		//cout << "changing." << endl;
		temp[pos] = s;
	}

	for (int i = 1; i < temp.size(); i++)
		states.push(temp[i]);	
}
//���״̬�Ƿ���close����
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