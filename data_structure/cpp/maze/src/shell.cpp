#include"shell.h"

Shell::Shell() {
	maze = NULL;
}

void Shell::init() {
	cout << "******************************" << endl;
	cout << "*         1.�����Թ�         *" << endl;
	cout << "*         2.�Թ�Ѱ·         *" << endl;
	cout << "******************************" << endl;
}

void Shell::read() {
	bool input = false;
	int act = 0;
	while (!input) {
		cout << "����������Ҫ�Ĺ���(����)��";
		cin >> act;
		switch (act)
		{
		case CREATE:
			delete maze;
			maze = new Maze();
			maze->printMaze();
			input = true;
			break;
		case SOLVE:
			if (maze == NULL) {
				cout << "�ֵܣ��㻹û�д����Թ���������һ�����ˣ�" << endl;
				maze = new Maze();
				maze->printMaze();
				input = true;
				break;
			}
			maze->solve();
			input = true;
			break;
		default:
			cout << "�ֵܣ�������벻��ͷ��" << endl;
			break;
		}
	}

}