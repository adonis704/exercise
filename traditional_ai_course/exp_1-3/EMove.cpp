#include"EMove.h"

EMove::EMove(string s) {
	open.init(s);
}

void EMove::cal() {
	//当未找到解时，不断对open表中的第一个状态扩展
	while (!open.isFinished() && !open.isOver()) {
		open.dequeue();
		open.expand();
	}

}

bool EMove::check(string s) {
	if (inverNum(s) %2 != inverNum(target) %2) return false;
	return true;
}

int EMove::inverNum(string s) {
	int res = 0;
	for (int i = 0; i < s.size() - 1; i++)
		for (int j = i + 1; j < s.size() - 1; j++)
			if ((int)s[j] < (int)s[i]) res++;
	return res;
}