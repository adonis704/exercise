#pragma once
#include<queue>
#include<stack>
#include<iostream>
#include"stateA.h"

//״̬��Ŀ��״̬�ľ���
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

//���ȶ��еıȽϲ���
struct cmp {
	bool operator() (StateA*& a, StateA*& b) const {
		return a->deep + a->dist > b->deep + b->dist;
	}
};

class ListA {
public:
	priority_queue<StateA*,vector<StateA*>,cmp> states;	//��С��������ȶ��У���open��
	vector<StateA*> close;	//close��
	vector<int> target;	//Ŀ��״̬
	StateA* cur;	//��ǰ�������״̬
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