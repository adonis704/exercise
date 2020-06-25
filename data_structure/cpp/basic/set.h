#include"list.h"
#define Set(T) Set<T>*
#define template(T) template<typename T>

template(T) class Set:public List<T> {
public:
	bool isEmpty(){return !this->size();}

	Set(T) operator &(Set<T>& x);
	Set(T) operator |(Set<T>& x);
	Set(T) operator -(Set<T>& x);
	//friend istream& operator>>(istream &is, const Set<T> &x);
	//friend ostream& operator<<(ostream &os, const Set<T> &x);
	bool operator==(Set<T>& x);
	T operator[](int i);

	bool have(const T& e);
	int find(const T& i);
	T add(const T& e);
	Set(T) copy();
	void clear();

};

template(T) int Set<T>::find(const T& i){
		NodePos(T) cur = this->first();
		int count = 0;
		for (int j = 0; j < this->size(); j++) {
			if (cur->data != i) {
				cur = cur->succ;
				count++;
			}
			else return count;
		}
	return -1;		
}

template(T) bool Set<T>::have(const T& i) {
	if (find(i) != -1)
		return true;
	else return false;
}

template(T)T Set<T>::add(const T& e) {
	if (have(e)) return NULL;
	else return this->insertAsLast(e)->data;
}

template(T) Set(T) Set<T>::copy() {
	Set(T) x = new Set<T>;
	NodePos(T) cur = this->first();
	for (int i = 0; i < this->size(); i++)
		x->add(cur->data);
	return x;
}

template(T)T Set<T>::operator[](int i) {
	NodePos(T) cur = this->first();
	for (int j = 0; j < i; j++)
		cur = cur->succ;
	return cur->data;
}

template(T)Set(T) Set<T>::operator&(Set<T>& x) {
	Set(T) res = copy();
	NodePos(T) cur = res->first();
	for (int i = 0; i < res->size(); i++) {
		if (!x.have(cur->data))
			res->remove(cur);
		cur = cur->succ;
	}

	return res;
}

template(T)Set(T) Set<T>::operator|(Set<T>& x) {
	Set(T) res = this->copy();
	NodePos(T) cur = x.first();
	for (int i = 0; i < x.size(); i++) {
		if (!have(cur->data))
			res->add(cur->data);
		cur = cur->succ;
	}

	return res;
}

template(T)Set(T) Set<T>::operator-(Set<T>& x) {
	Set(T) res = this->copy();
	NodePos(T) cur = res->first();
	for (int i = 0; i < res->size(); i++) {
		if (x.have(cur->data))
			res->remove(cur);
		cur = cur->succ;
	}

	return res;
}

//template(T)istream& operator>>(istream &is, const Set<T> &x) {
//	while (is) {
//		T temp;
//		is >> temp;
//		x->add(temp);
//	}
//	return is;
//}
//
//template(T)ostream& operator<<(ostream &os, const Set<T> &x) {
//	for (int cur = 0; cur < x->size(); cur++)
//		os << x[cur];
//	return os;
//}

template(T) bool Set<T>::operator==(Set<T>& x) {
	if (this->size() != x.size()) return false;
	NodePos(T) cur = this->first();
	for (int i = 0; i < this->size(); i++)
		if (!x.have(cur->data))
			return false;
	return true;
}

template(T) void Set<T>::clear() {
	while (this->size())
		remove(this->first());
	delete[] this;

}

