#include"shell.h"

int main() {
	Shell shell;
	shell.init();
	while (true)
		shell.read();
	system("pause");
}