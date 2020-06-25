#pragma once
#include<vector>
#ifndef NULL
#define NULL 0
#endif 
#define parent(i) ((i-1)/2)
#define lchild(i) (1+2*i)
#define rchild(i) ((i+1)*2)
using namespace std;

template <typename T> class Heap {
private:
	vector<T> heap;

	void down();
	void up();

public:
	Heap();
	void insert(T e);
	int size();
	T pop();
	T front();

};