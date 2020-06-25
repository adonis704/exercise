#ifndef testShell_h
#define testShell_h
#include"process.h"
typedef enum {
	WRONG,
	INITIAL,
	CREATE,
	DESTROY,
	REQUEST,
	RELEASE,
	TIME_OUT,
	LIST_READY,
	LIST_BLOCK,
	LIST_RESOURCES,
	PROVIDE
} COMMAND;

class TestShell {
private:
	Process* process;	//��ʼ��һ������������ȹ��̵Ķ���

	void explain(COMMAND com, string n1, string n2);
public:
	TestShell();
	~TestShell();
	void read();
};

#endif