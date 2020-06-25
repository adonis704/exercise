#ifndef shell_h
#define shell_h
#include"maze.h"

typedef enum {
	CREATE = 1,
	SOLVE = 2
} ACT;

class Shell {
private:
	Maze* maze;
public:
	Shell();
	void init();
	void read();
};

#endif
