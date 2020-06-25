#include"array.h"

template(T)ListNode<T> ListNode<T>::operator=(T e) {
	data = e;
	return *this;
}

template(T)NodePos(T) ListNode<T>::insertAsPred(const T& e) {
	NodePos(T) x = new ListNode<T>(e, this->pred, this);
	this->pred->succ = x;
	this->pred = x;
	return x;
}

template(T)NodePos(T) ListNode<T>::insertAsSucc(const T& e) {
	NodePos(T) x = new ListNode<T>(e, this, this->succ);
	this->succ->pred = x;
	this->succ = x;
	return x;
}

template(T)T List<T>::remove(NodePos(T) x) {
	T res = x->data;
	x->pred->succ = x->succ;
	x->succ->pred = x->pred;
	delete[] x;
	_size--;
	return res;
}

template(T)List<T>::List() {
	_size = 0;
	header = new ListNode<T>;
	trailer = new ListNode<T>;
	header->succ = trailer; header->pred = NULL;
	trailer->pred = header; trailer->succ = NULL;
}

