#ifndef avoidSprite_h
#define avoidSprite_h
#include<math.h>
#include"autoSprite.h"
#include"userSprite.h"
const int SPEED_AVOID = 20;
const int GAP_VALUE = 6;
const int DIST_SAFE = 200;

class AvoidSprite:public AutoSprite {
private:
	UserSprite* target;
	int gap;

public:
	AvoidSprite(int x, int y, int w, int h, ACL_Image& img, UserSprite* t);
	~AvoidSprite();

	bool danger();
	void move();
};
#endif