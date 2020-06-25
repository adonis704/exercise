#include"mcProblem.h"

McProblem::McProblem(int m, int c, int n) {
	f1 = new Function(m, c, n);
}

void McProblem::cal() {
	while (!f1->isFinished() && !f1->isOver())
		f1->expand();
}