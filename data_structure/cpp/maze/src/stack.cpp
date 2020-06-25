#include"stack.h"

template(T) NodePos(T) Stack<T>::pop() {
	NodePos(T) temp = new ListNode<T>();
	if (this->_size < 1) return  temp;
	temp = this->last();
	this->_size--;
	this->trailer->pred = temp->pred;
	temp->pred->succ = this->trailer;
	return temp;
}

template(T) void Stack<T>::push(NodePos(T) x) {
	this->_size++;
	NodePos(T) temp = this->last();
	x->succ = temp->succ;
	x->pred = temp;
	temp->succ->pred = x;
	temp->succ = x;
}

template(T) void Stack<T>::push(T e) {
	NodePos(T) temp = new ListNode<T>(e);
	push(temp);
}