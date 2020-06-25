#include"node.h"

Node::Node(int n, int w, vector<Node*> a, vector<int> c) {
	name = n;
	weight = w;
	available = a;
	cost = c;
}