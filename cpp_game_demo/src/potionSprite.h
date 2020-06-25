#ifndef potionSprite_h
#define potionSprite_h

#include"autoSprite.h"

class PotionSprite :public AutoSprite {
public:
	PotionSprite(int x, int y, int w, int h, ACL_Image& img);
	~PotionSprite();
	void move();
};

#endif
