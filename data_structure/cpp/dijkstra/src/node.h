#pragma once
#include"heap.h"
#include"heap.cpp"
const int infinity = 65535;
class Node {
public:
	int name;	//��������
	Node* parent;	//ͨ·�е���һ������
	int weight;	//�ܿ���
	vector<Node*> available;	//�ɴ����
	vector<int> cost;	//�����Ӧ���еĿ���

	Node(int n, int w = infinity, vector<Node*> a = { NULL }, vector<int> c = {-1});
	
};