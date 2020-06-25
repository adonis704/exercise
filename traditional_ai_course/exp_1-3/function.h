#pragma once
#include<iostream>
#include<string>
#include<queue>
#include<stack>
#include<cmath>
#include"state.h"

const int MEMBER_MAX = 110;
class Function {
private:
	vector<string> possible;	//可能的状态
	vector<vector<int>> oprt;	//操作符
	State* header; State* root;	//头状态，第一个状态
	queue<State*> running;	//可以扩展的状态
	bool finish, over;

public:
	Function(int mMax, int cMax, int nMax);
	~Function();
	vector<string>::iterator check(int ms, int cs, int ns);
	void expand();
	State* leftToRight(State* cur);
	void rightToLeft(State* cur);
	bool isFinished();
	bool isOver();
	void print(State* cur);
};