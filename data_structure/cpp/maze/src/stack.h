#ifndef stack_h
#define stack_h
#include"list.h"
#include"list.cpp"

template(T) class Stack:public List<T> {
public:
	NodePos(T) pop();
	void push(NodePos(T) x);
	void push(T e);
};

#endif
