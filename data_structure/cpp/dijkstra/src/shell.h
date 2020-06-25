#pragma once
#include"dijkstra.h"
class Shell {
private:
	Dijkstra* dijkstra;
	bool existed;

public:
	Shell();
	void init();
	void generate();
	void solve();
};