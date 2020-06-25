#include"mazeNode.h"

MazeNode::MazeNode(int posx, int posy) {
	x = posx; y = posy;
	for (int i = 0; i < 4; i++) 
		move.push_back(1);
}

vector<int> MazeNode::act() {
	vector<int> res;
	//ср
	if (move[0] == 1) {
		res.push_back(0);
		res.push_back(1);
		move[0] = 0;
	}
	//об
	else if (move[1] == 1) {
		res.push_back(1);
		res.push_back(0);
		move[1] = 0;
	}
	//вС
	else if (move[2] == 1) {
		res.push_back(0);
		res.push_back(-1);
		move[2] = 0;
	}
	//ио
	else if (move[3] == 1) {
		res.push_back(-1);
		res.push_back(0);
		move[3] = 0;
	}
	else {
		res.push_back(0);
		res.push_back(0);
	}
	return res;
}