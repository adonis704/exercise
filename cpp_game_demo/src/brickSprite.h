#ifndef brickSprite_h
#define brickSprite_h

#include"autoSprite.h"
const int LIFE = 300;
class BrickSprite : public AutoSprite {
public:
	BrickSprite(int x, int y, int w, int h, ACL_Image& img);
	~BrickSprite();
	void move();
};

#endif
