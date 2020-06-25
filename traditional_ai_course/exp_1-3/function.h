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
	vector<string> possible;	//���ܵ�״̬
	vector<vector<int>> oprt;	//������
	State* header; State* root;	//ͷ״̬����һ��״̬
	queue<State*> running;	//������չ��״̬
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