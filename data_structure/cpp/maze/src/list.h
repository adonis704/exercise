#ifndef list_h
#define list_h
#include"listNode.h"
#include"listNode.cpp"

template(T) class List {
protected:
	int _size; NodePos(T) header; NodePos(T) trailer;

public:
	List();
	int size();
	NodePos(T) first();
	NodePos(T) last();
	NodePos(T) insertAsLast(const T& e);
	NodePos(T) insertAsFirst(const T& e);
	NodePos(T) insertAsPred(const T& e, NodePos(T) x);
	NodePos(T) insertAsSucc(const T& e, NodePos(T) x);
	T remove(NodePos(T) x);

};


#endif
