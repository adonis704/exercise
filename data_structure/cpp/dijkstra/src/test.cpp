#include"shell.h"

int main() {
	Shell shell;
	shell.init();
	int func;
	while (true) {
		cout << "�����빦�ܣ����֣���";
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