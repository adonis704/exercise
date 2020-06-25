#pragma once
#include<queue>
#include<stack>
#include<iostream>
#include"stateA.h"

//状态到目标状态的距离
const int dist[9][9] = {
	{0,1,2,1,2,3,2,3,4},
	{1,0,1,2,1,2,3,2,3},
	{2,1,0,3,2,1,4,3,2},
	{1,2,3,0,1,2,1,2,3},
	{2,1,2,1,0,1,2,1,2},
	{3,2,1,2,1,0,3,2,1},
	{2,3,4,1,2,3,0,1,2},
	{3,2,3,2,1,2,1,0,1},
	{4,3,2,3,2,1,2,1,0}
};

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
}	DIRECTION;

//优先队列的比较操作
struct cmp {
	bool operator() (StateA*& a, StateA*& b) const {
		return a->deep + a->dist > b->deep + b->dist;
	}
};

class ListA {
public:
	priority_queue<StateA*,vector<StateA*>,cmp> states;	//由小到大的优先队列，即open表
	vector<StateA*> close;	//close表
	vector<int> target;	//目标状态
	StateA* cur;	//当前待处理的状态
	bool finish, over;

	ListA();
	void add(StateA* x);
	StateA* dequeue();
	int distance(vector<int> s);
	int find(int num);
	vector<string>::iterator find(StateA* s);
	int find(vector<int> s);
	void expand();
	void print(StateA* s);
	void move();
	void exam(StateA* s, int pos);
	void init(string s);
	bool isFinished();
	bool isOver();
	bool done(vector<int> s);

};