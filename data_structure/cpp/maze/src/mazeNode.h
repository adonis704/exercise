#ifndef mazeNode_h
#define mazeNode_h
#include<vector>
using namespace std;

class MazeNode {
public:
	int x, y;
	vector<int> move;

	MazeNode(int posx, int posy);
	vector<int> act();
};

#endif