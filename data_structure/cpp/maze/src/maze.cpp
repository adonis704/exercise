#include"maze.h"

Maze::Maze() {
	int x, y;
	vector<int> temp;
	for (int i = 0; i < MAX_Y; i++)
		temp.push_back(1);
	for (int i = 0; i < MAX_X; i++)
		map.push_back(temp);

	srand((unsigned)time(NULL));
	for (int i = 0; i < AVAILABLE; i++)	
	{
		x = rand() % (MAX_X - 2) + 1;
		y = rand() % (MAX_Y - 2) + 1;
		map[x][y] = 0;
	}

	map[0][1] = 2;
	map[1][1] = 0;
	map[MAX_X - 1][MAX_Y - 2] = 3;
	map[MAX_X - 2][MAX_Y - 2] = 0;
}

void Maze::printMaze() {
	for (int i = 0; i < MAX_X; i++) {
		for (int j = 0; j < MAX_Y; j++) {
			if (map[i][j] == 0 || map[i][j] == -1) cout << "  ";
			else if (map[i][j] == 1) cout << "■";
			else if (map[i][j] == 2) cout << "●";
			else if (map[i][j] == 3) cout << "★";
			else if (map[i][j] == UP) cout << "↑";
			else if (map[i][j] == DOWN) cout << "↓";
			else if (map[i][j] == LEFT) cout << "←";
			else if (map[i][j] == RIGHT) cout << "→";
		}			
		cout << endl;
	}
	cout << "- - - - - - - - - - - - - - - - - - - - - " << endl;

}

void Maze::solve() {
	MazeNode* start = new MazeNode(0, 1);
	stack.push(start);
	NodePos(MazeNode*) temp = NULL;
	vector<int> move;
	int x, y;
	bool suck = false;
	bool finish = false;

	while (stack.size() > 0) {
		//出栈
		temp = stack.pop();
		x = temp->data->x;
		y = temp->data->y;

		//找到当前节点下一个可达节点，如果没有，则再次出栈
		while (!check(temp, move = temp->data->act()))
			if (move[0] == 0 && move[1] == 0) {
				map[x][y] = -1;
				suck = true;
				break;
			}
		if (suck) {
			suck = false;
			continue;
		}

		//找到下一个可达节点
		MazeNode* newNode = new MazeNode(x + move[0], y + move[1]);
		stack.push(temp);
		stack.push(newNode);
		map[newNode->x][newNode->y] = -1;

		//判断下一个可达节点是否为终点，若是，则完成寻路
		if (newNode->x == MAX_X - 1 && newNode->y == MAX_Y - 2) {
			finish = true;
			break;
		}
	}

	if (finish) setDirection();
	else cout << "No solution." << endl;
}

bool Maze::check(NodePos(MazeNode*) target, vector<int> move) {
	int x = target->data->x + move[0];
	int y = target->data->y + move[1];
	if (x == MAX_X - 1 && y == MAX_Y - 2) return true;
	if (x < 1 || y < 1 || x > MAX_X-2 || y > MAX_Y-2 ) return false;
	if (map[x][y] == -1 || map[x][y] == 1) return false;
	return true;
}

void Maze::setDirection() {
	vector<NodePos(MazeNode*)> path;
	while (stack.size() > 0)
		path.push_back(stack.pop());
	DIRECTION direction;
	int x,y,xx, yy;
	for (int i = 0; i < path.size() - 2; i++) {
		x = path[i+1]->data->x;
		y = path[i+1]->data->y;
		xx = path[i]->data->x - path[i + 1]->data->x;//x坐标的差值
		yy = path[i]->data->y - path[i + 1]->data->y;//y坐标的差值
		if (xx == 1) direction = DOWN;
		else if (xx == -1) direction = UP;
		else if (yy == 1) direction = RIGHT;
		else direction = LEFT;
		map[x][y] = direction;
	}
	map[MAX_X - 1][MAX_Y - 2] = 3;
	printMaze();
}