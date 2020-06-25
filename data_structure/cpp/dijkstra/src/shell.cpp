#include"shell.h"

Shell::Shell() {
	existed = false;
}

void Shell::init() {
	cout << "******************" << endl;
	cout << "*   1.���ɳ���   *" << endl;
	cout << "*   2.����ͨ·   *" << endl;
	cout << "******************" << endl;
}

void Shell::generate() {
	if (existed) delete dijkstra;
	int num;
	while (true) {
		cout << "�������������";
		cin >> num;
		if (num > 0) break;
	}
	dijkstra = new Dijkstra(num);
	dijkstra->print();
	existed = true;
}

void Shell::solve() {
	if (!existed) return;
	int size = dijkstra->size() ;
	int start, end;
	while (true) {
		cout << "��������㣬�յ㣺";
		cin >> start >> end;
		if (start == end) {
			cout << "- - - - - - - - - - - - - - -" << endl;
			cout << "ֱ�� ����:0" << endl;
			cout << "- - - - - - - - - - - - - - -" << endl;
			return;
		}
		if (start >= 0 && end >= 0 && end < size) break;
	}
	dijkstra->solve(start, end);
}