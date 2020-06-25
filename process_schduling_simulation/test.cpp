#include"testShell.h"
int main() {
	//Process p1;
	//p1.init();
	//p1.create("x", USER);
	//p1.create("p", USER);
	//p1.create("q", USER);
	//p1.create("r", USER);
	//p1.listReady();
	//p1.timeOut();
	//p1.request(R2, 1);
	//p1.timeOut();
	//p1.request(R3, 3);
	//p1.timeOut();
	//p1.request(R4, 3);
	//p1.listAvailable();
	//p1.timeOut();
	//p1.timeOut();
	//p1.request(R3, 1);
	//p1.request(R4, 2);
	//p1.request(R2, 2);
	//p1.listBlocked();
	//p1.listAvailable();
	//p1.timeOut();
	//p1.provide("q");
	//p1.provide("p");
	//p1.destroy("q");
	//p1.listAvailable();
	//p1.provide("p");
	//p1.timeOut();
	//p1.timeOut();
	//system("pause");
	
	TestShell ts;
	while (true)
	{
		ts.read();
		cout << endl;
	}
}