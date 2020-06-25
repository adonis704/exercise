#include"list.h"

template(T) List<T>::List() {
	_size = 0;
	header = new ListNode<T>;
	trailer = new ListNode<T>;
	header->succ = trailer; header->pred = NULL;
	trailer->pred = header; trailer->succ = NULL;
}

template(T) int List<T>::size() { return _size; }
template(T) NodePos(T) List<T>::first() { return header->succ; }
template(T) NodePos(T) List<T>::last() { return trailer->pred; }

template(T) NodePos(T) List<T>::insertAsFirst(const T& e) { 
	_size++; 
	return header->insertAsSucc(e);
}

template(T) NodePos(T) List<T>::insertAsLast(const T& e) { 
	_size++; 
	return trailer->insertAsPred(e); 
}

template(T)NodePos(T) List<T>::insertAsPred(const T& e, NodePos(T) x) {
	_size++; 
	return x->insertAsPred(e);
}

template(T)NodePos(T) List<T>::insertAsSucc(const T& e, NodePos(T) x) {
	_size++;
	return x->insertAsSucc(e);
}

template(T)T List<T>::remove(NodePos(T) x) {
	T res = x->data;
	x->pred->succ = x->succ;
	x->succ->pred = x->pred;
	delete[] x;
	_size--;
	return res;
}