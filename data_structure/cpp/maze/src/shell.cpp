#include"shell.h"

Shell::Shell() {
	maze = NULL;
}

void Shell::init() {
	cout << "******************************" << endl;
	cout << "*         1.生成迷宫         *" << endl;
	cout << "*         2.迷宫寻路         *" << endl;
	cout << "******************************" << endl;
}

void Shell::read() {
	bool input = false;
	int act = 0;
	while (!input) {
		cout << "请输入您需要的功能(数字)：";
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
				cout << "兄弟，你还没有创建迷宫，我送你一个好了！" << endl;
				maze = new Maze();
				maze->printMaze();
				input = true;
				break;
			}
			maze->solve();
			input = true;
			break;
		default:
			cout << "兄弟，你的输入不对头！" << endl;
			break;
		}
	}

}