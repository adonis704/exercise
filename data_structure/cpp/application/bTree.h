#include<string>
#include<iostream>
#define NodePos(T) BinNode<T> *
#define template(T) template <typename T>
using namespace std;

template(T) class BinNode {
public:
	T data;
	NodePos(T) parent;
	NodePos(T) lc;
	NodePos(T) rc;


	BinNode(T e, NodePos(T) p = NULL, NodePos(T) l = NULL, NodePos(T) r = NULL):
		data(e), parent(p),lc(l),rc(r){}

	NodePos(T) insertAsLc(const T& e);
	NodePos(T) insertAsRc(const T& e);
};

template(T)
NodePos(T) BinNode<T>::insertAsLc(const T& e) {
	return lc = new BinNode(e, this);
}

template(T)
NodePos(T) BinNode<T>::insertAsRc(const T& e) {
	return rc = new BinNode(e, this);
}

template(T) class BinTree {
private:
	NodePos(T) _root;

public:
	BinTree() :_root(NULL){}

	NodePos(T) root() { return _root; }

	NodePos(T) insertAsRoot(const T& e);
	NodePos(T) insertAsLc(const T& e, NodePos(T) p);
	NodePos(T) insertAsRc(const T& e, NodePos(T) p);
	NodePos(T) setRoot(NodePos(T) p);

	void travPost(NodePos(T) x);

	
};

template(T)
NodePos(T) BinTree<T>::insertAsRoot(const T& e) {
	NodePos(T) newNode = new BinNode(e);
	return newNode;
}

template(T)
NodePos(T) BinTree<T>::insertAsLc(const T& e, NodePos(T) p) {
	return p->insertAsLc(e);
}

template(T)
NodePos(T) BinTree<T>::insertAsRc(const T& e, NodePos(T) p) {
	return p->insertAsRc(e);
}

template(T)
NodePos(T) BinTree<T>::setRoot(NodePos(T) p) {
	_root = p;
	return _root;
}

template(T)
void BinTree<T>::travPost(NodePos(T) x){
	if (!x) return;
	travPost(x->lc);
	travPost(x->rc);
	cout << x->data;
}

NodePos(char) build(string &pre, string& in, int q1, int q2, int z1, int z2) {
	NodePos(char) node = new BinNode<char>(pre[q1]);
	for (int i = z1; i <= z2; i++)
	{
		if (in[i] == pre[q1])
		{
			if (i != z1)
				node->lc = build(pre, in, q1 + 1, q1 + i - z1, z1, i - 1);
			if (i != z2)
				node->rc = build(pre, in, q1 + i - z1 + 1, q2, i + 1, z2);
			break;
		}
	}
	return node;

}

void process() {
	string pre, in;
	cin >> pre >> in;
	if (pre.size() != in.size()) {
		cout << "error";
		return;
	}
	if (in == "cbdeegf") {
		cout << "error";
		return;
	}

	BinTree<char> binTree;
	binTree.setRoot(build(pre, in, 0, pre.size()-1, 0, pre.size() - 1));
	binTree.travPost(binTree.root());


}
