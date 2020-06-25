#include"shell.h"

int main() {
	Shell shell;
	shell.init();
	int func;
	while (true) {
		cout << "请输入功能（数字）：";
		cin >> func;
		switch (func)
		{
		case 1:
			shell.generate();
			break;
		case 2:
			shell.solve();
			break;
		default:
			break;
		}
	}
}