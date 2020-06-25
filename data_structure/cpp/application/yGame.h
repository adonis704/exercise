#include<iostream>
#include <strstream>
#include<string>
#include <cctype>
#define NodePos(T) ListNode<T>*
#define template(T) template <typename T>
using namespace std;

template(T)
class ListNode {
public:
	T data;
	NodePos(T) pred;
	NodePos(T) succ;
	
	ListNode() {};
	ListNode(T e, NodePos(T) p = NULL, NodePos(T) s = NULL):
		data(e), pred(p), succ(s){}

	NodePos(T) insertAsPred(const T &e);
	NodePos(T) insertAsSucc(const T &e);

};

template(T)
NodePos(T) ListNode<T>::insertAsPred(const T &e) {
	NodePos(T) newNode = new ListNode(e, pred, this);
	pred->succ = newNode;
	pred = newNode;
	return newNode;
}

template(T)
NodePos(T) ListNode<T>::insertAsSucc(const T& e) {
	NodePos(T) newNode = new ListNode(e, this, succ);
	succ->pred = newNode;
	succ = newNode;
	return newNode;
}

template(T)
class List {
private:
	int _size;
	NodePos(T) header;
	NodePos(T) trailer;

public:
	List() { init(); };
	void init();
	NodePos(T) insertAsFirst(const T& e);
	NodePos(T) insertAsLast(const T& e);
	NodePos(T) insertAsPred(const T& e, NodePos(T) p);
	NodePos(T) insertAsSucc(const T& e, NodePos(T) p);
	T remove(NodePos(T) nodePos);

	int size() { return _size; };
	NodePos(T) first() { return header->succ; };
	NodePos(T) last() { return trailer->pred; };

};

template(T)
void List<T>::init() {
	_size = 0;
	header = new ListNode<T>;
	trailer = new ListNode<T>;
	header->pred = NULL;
	header->succ = trailer;
	trailer->succ = NULL;
	trailer->pred = header;
}

template(T)
NodePos(T) List<T>::insertAsFirst(const T& e) {
	header->insertAsSucc(e);
	_size++;
}

template(T)
NodePos(T) List<T>::insertAsLast(const T& e) {
	trailer->insertAsPred(e);
	_size++;
}

template(T)
NodePos(T) List<T>::insertAsPred(const T& e, NodePos(T) p) {
	_size++;
	return 	p->insertAsPred(e);
}

template(T)
NodePos(T) List<T>::insertAsSucc(const T& e, NodePos(T) p) {
	_size++;
	return 	p->insertAsSucc(e);
}

template(T)
T List<T>::remove(NodePos(T) nodePos) {
	T e = nodePos->data;
	nodePos->pred->succ = nodePos->succ;
	nodePos->succ->pred = nodePos->pred;
	delete nodePos;
	_size--;
	return e;
}

void game(int n,int k,int m) {
	if (k > n  || n <1 ){
		cout << "ERROR";
		return;
	}

	if ( k == 0 && m == 0) {
		cout << 0;
		return;
	}
		

	List<int> list;
	NodePos(int) curTemp = list.last();
	for (int i = 0; i < n; i++) 
		curTemp = list.insertAsSucc(i + 1, curTemp);

	NodePos(int) cur = list.first();
	NodePos(int) first = list.first();
	list.last()->succ = list.first();
	list.first()->pred = list.last();
	
	
	while (list.size() > k) {
		int count = 0;
		while (count++ != m)
			cur = cur->succ;
		if (cur == first)
			first = cur->succ;
		NodePos(int) curNext = cur->succ;
		list.remove(cur);
		cur = curNext;
	}

	for (int i = 0; i != list.size(); i++) {
		if (i != list.size() - 1)
			cout << first->data << ",";
		else
			cout << first->data ;
		first = first->succ;
	}

	}

bool check(char arr[]) {
	for (int i = 0; arr[i] != '\0'; i++)
		if (!isdigit(arr[i]))
			return false;
	return true;
}

int convertStringToInt(const string &s)
{
	int val;
	strstream ss;
	ss << s;
	ss >> val;
	return val;
}

int toInt(char arr[]) {
	string temp = "";
	for (int i = 0; arr[i] != '\0'; i++)
		temp += arr[i];
	int res = convertStringToInt(temp);
	return res;
}


void process() {
	char n[5], k[5], m[5];
	cin.getline(n, 5, ' ');
	cin.getline(k, 5, ' ');
	cin.getline(m, 5);
	if (!check(n) || !check(k) || !check(m))
		cout << "ERROR";
	else {
		int ni = toInt(n);
		int ki = toInt(k);
		int mi = toInt(m);
		game(ni, ki, mi);
	}
	
}
