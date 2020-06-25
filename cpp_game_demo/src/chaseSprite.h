#ifndef chaseSprite_h
#define chaseSprite_h

#include"autoSprite.h"
#include"userSprite.h"
const int SPEED_CHASE = 1;
const int TAN_MAX = 5;

class ChaseSprite :public AutoSprite {
private:
	UserSprite* target;
	int gap;

public:
	ChaseSprite(int x, int y, int w, int h, ACL_Image& img, UserSprite* t);
	~ChaseSprite();

	void move();
};

#endif