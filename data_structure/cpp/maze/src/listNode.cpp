#include"listNode.h"
template(T)ListNode<T>::ListNode(){}
template(T) ListNode<T>::ListNode(T e, NodePos(T) p, NodePos(T) s) {
	data = e; pred = p; succ = s;
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

template(T) ListNode<T> ListNode<T>::operator= (T e) {
	data = e;
	return *this;
}

//template(T1) std::ostream& operator<<(std::ostream& os, ListNode<T1>& node){
//	os << node.data;
//	return os;
//}
