#ifndef maze_h
#define maze_h
#include<ctime>
#include"stack.h"
#include"stack.cpp"
#include"mazeNode.h"

typedef enum {
	UP = 6,
	DOWN = 7,
	LEFT = 8,
	RIGHT = 9
} DIRECTION;

const int MAX_X = 20;
const int MAX_Y = 20;
const int AVAILABLE = 400;

class Maze {
private:
	vector<vector<int>> map;
	Stack<MazeNode*> stack;

public:
	Maze();
	void printMaze();
	void solve();
	bool check(NodePos(MazeNode*) x, vector<int> m);
	void setDirection();
};

#endif