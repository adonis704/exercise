#include"testShell.h"
//初始化
TestShell::TestShell() {
	process = new Process();
}
TestShell::~TestShell() {

}

//从终端读取指令
void TestShell::read() {
	COMMAND command = WRONG;
	string c1, c2, c3;

	//先读入第一个操作数
	cin >> c1;
	if (c1 == "init") command = INITIAL;	//指令赋值
	else if (c1 == "to") command = TIME_OUT;
	//如果还有第二个操作数，继续读入
	else {
		cin >> c2;
		if (c1 == "de") command = DESTROY;
		else if (c1 == "pr") command = PROVIDE;
		else if (c1 == "list" && c2 == "ready") command = LIST_READY;
		else if (c1 == "list" && c2 == "block") command = LIST_BLOCK;
		else if (c1 == "list" && c2 == "res") command = LIST_RESOURCES;
		//如果还有第三个操作数，继续读入
		else {
			cin >> c3;
			if (c1 == "cr") command = CREATE;
			else if (c1 == "req") command = REQUEST;
			else if (c1 == "rel") command = RELEASE;
		}
	}

	explain(command, c2, c3);
}

//解析指令
void TestShell::explain(COMMAND com, string n1, string n2) {
	int num = 0;	//请求或释放资源的数量
	RESOURCE_TYPE r;	//请求资源种类
	PCB_PRIORITY po;	//进程优先级

	//如果有必要，翻译n1和n2
	if (n1 == "R1") r = R1;
	else if (n1 == "R2") r = R2;
	else if (n1 == "R3") r = R3;
	else if (n1 == "R4") r = R4;
	if (n2 == "1") { num = 1; po = USER; }
	else if (n2 == "2") { num = 2; po = SYSTEM; }
	else if (n2 == "3") num = 3;
	else if (n2 == "4") num = 4;

	//调用相应方法
	switch (com) {
	case INITIAL:
		process->init();
		break;
	case CREATE:
		process->create(n1, po);
		break;
	case DESTROY:
		process->destroy(n1);
		break;
	case REQUEST:
		process->request(r, num);
		break;
	case RELEASE:
		process->release(r, num);
		break;
	case TIME_OUT:
		process->timeOut();
		break;
	case LIST_READY:
		process->listReady();
		break;
	case LIST_BLOCK:
		process->listBlocked();
		break;
	case LIST_RESOURCES:
		process->listAvailable();
		break;
	case PROVIDE:
		process->provide(n1);
		break;
	}
}
