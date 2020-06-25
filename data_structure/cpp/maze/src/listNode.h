#ifndef listNode_h
#define listNode_h
#include<iostream>
#define template(T) template<typename T>
#define NodePos(T) ListNode<T>*
using namespace std;

template(T) class ListNode {
public:
	T data; NodePos(T) pred; NodePos(T) succ;

	ListNode();
	ListNode(T e , NodePos(T) p = NULL, NodePos(T) s = NULL);

	NodePos(T) insertAsPred(const T& e);
	NodePos(T) insertAsSucc(const T& e);
	ListNode<T> operator=(T e);
	//template(T1) friend std::ostream& operator<<(std::ostream& os, ListNode<T1>& node);
};


#endif