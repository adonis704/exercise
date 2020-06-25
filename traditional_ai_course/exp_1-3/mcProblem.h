#pragma once
#include"function.h"

class McProblem {
private:
	Function* f1;

public:
	McProblem(int m,int c, int n);
	void cal();
};