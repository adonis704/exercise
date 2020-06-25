#ifndef autoSprite_h
#define autoSprite_h
#include"baseSprite.h"
const int WINWIDTH__ = 1200;
const int WINHEIGHT__ = 800;
const int SPEED_AUTO = 10;

class AutoSprite:public BaseSprite {
protected:
	int dx, dy;

public:
	int life;
	AutoSprite(int x, int y, int w, int h, ACL_Image& img);
	~AutoSprite();
	virtual void move();

};

#endif