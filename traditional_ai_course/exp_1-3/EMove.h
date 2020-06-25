#pragma once
#include"listA.h"
const string target = "123804765";

class EMove {
private:
	ListA open;
	int inverNum(string s);

public:
	EMove(string s);
	void cal();
	bool check(string s);

};