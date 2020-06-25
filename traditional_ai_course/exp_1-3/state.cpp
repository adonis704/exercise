#include"state.h"
#include<iostream>
State::State(int ms, int cs , int ns, State* p, int d) {
	m = ms;	c = cs;	n = ns;
	deep = d;
	parent = p;
}

State::~State(){}

State* State::setChildren(int ms, int cs, int ns) {
	State* res = new State(ms, cs, ns, this,deep+1);
	children.push_back(res);
	return res;
}