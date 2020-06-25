#ifndef array_h
#define array_h
#include<iostream>
#define NodePos(T) ListNode<T>*
#define template(T) template<typename T>
using namespace std;

template(T) class ListNode {
public:
	T data; NodePos(T) pred; NodePos(T) succ;

	ListNode() {}
	ListNode(T e, NodePos(T) p = NULL, NodePos(T) s = NULL) :data(e), pred(p), succ(s) {}

	NodePos(T) insertAsPred(const T& e);
	NodePos(T) insertAsSucc(const T& e);
	ListNode<T> operator=(T e);
};

template(T) class List {
protected:
	int _size; NodePos(T) header; NodePos(T) trailer;

public:
	List();
	~List() {}
	int size() { return _size; }
	T remove(NodePos(T) x);
	NodePos(T) first() { return header->succ; }
	NodePos(T) last() { return trailer->pred; }
	NodePos(T) insertAsLast(const T& e) { _size++; return trailer->insertAsPred(e); }
	NodePos(T) insertAsFirst(const T& e) { _size++; return header->insertAsSucc(e); }
	NodePos(T) insertAsPred(const T& e, NodePos(T) x) { _size++; return x->insertAsPred(e); }
	NodePos(T) insertAsSucc(const T& e, NodePos(T) x) { _size++; return x->insertAsSucc(e); }

};


template(T) class Array:public List<T> {
public:
	NodePos(T) operator[](int i) {
		NodePos(T) cur = this->header;
		for (int j = 0; j < i; j++)
			cur = cur->succ;
		return cur;
	}
};

#endif





