#include<iostream>
#define template(T) template<class T>
#define CAPACITY 10
using namespace std;

//template(T) class Set;
template(T) class Set {
private:
	int _size; int _capacity; 

protected:
	void expand();

public:
	template(T1) friend Set<T1> operator&(Set<T1>&x, Set<T1>&y);
	T* _elem;
	Set() { init(); }
	~Set() {}

	int size() { return _size; }
	void init();
	void add(const T& e);
	int have(const T& e);

	T operator[](int i);
	void remove(const T& e);
	Set<T> copy();
	void clear();

	template<class T2> friend ostream& operator<< (ostream& os, Set<T>& x);
	template<class T2> friend istream& operator>> (istream& is, Set<T>& x);


};

template(T1)Set<T1> operator&(Set<T1>&x, Set<T1>&y) {
	Set<T1> res = x.copy();
	for (int i = 0; i < x.size(); i++)
		if (y.have(x._elem[i]) == -1)
			res.remove(x._elem[i]);
	return res;
}

template(T1)Set<T1> operator|(Set<T1>&x, Set<T1>&y) {
	Set<T1> res = x.copy();
	for (int i = 0; i < y.size(); i++)
		res.add(y[i]);
	return res;
}

template(T)void Set<T>::init() {
	_size = 0; _capacity = CAPACITY; _elem = new T[_capacity];
}

template(T) void Set<T>::expand() {
	if (_size >= _capacity) {
		T* temp = new T[_capacity * 2];
		for (int i = 0; i < _size; i++)
			temp[i] = _elem[i];
		clear();
		_elem = temp;
	}
}

template(T) int Set<T>::have(const T& e) {
	int pos = 0;
	for (; pos < _size; pos++)
		if (_elem[pos] == e)
			return pos;
	return -1;
}

template(T)void Set<T>::remove(const T& e) {
	int pos = -2;
	if ((pos = have(e)) == -1) return;
	for (; pos < _size - 1; pos++)
		_elem[pos] = _elem[pos + 1];
	_size--;
}

template(T) void Set<T>::clear() {
	delete[] _elem;
	init();
}

template(T)void Set<T>::add(const T& e) {
	if (have(e) != -1) return;
	expand();
	_elem[_size++] = e;
}

template(T)Set<T> Set<T>::copy() {
	Set<T> res;
	for (int i = 0; i < _size; i++)
		res.add(_elem[i]);
	return res;
}



template(T)T Set<T>::operator[](int i) {
	return _elem[i];
}

template(T)ostream& operator<<(ostream& os, Set<T>& x) {
	for (int i = 0; i < x.size(); i++)
		os << x[i] << " ";
	return os;
}

template(T)istream& operator>>(istream& is, Set<T>& x) {
	T temp;
	while (is >> temp)
		x.add(temp);
	return is;
}