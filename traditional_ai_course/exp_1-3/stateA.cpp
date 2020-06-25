#include"stateA.h"

//不可缺省属性：状态，距离，深度，父结点
StateA::StateA(vector<int> s,int d, int de, StateA* p) {
	parent = p;
	dist = d; deep = de;
	state = s;
}
