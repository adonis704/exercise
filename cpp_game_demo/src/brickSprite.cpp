#include"brickSprite.h"

BrickSprite::BrickSprite(int x, int y, int w, int h, ACL_Image& img) :
	AutoSprite(x, y, w, h, img) {
	type = BRICK;
	life = LIFE;
}

BrickSprite::~BrickSprite(){}

void BrickSprite::move() {
	if (life > 0)
		life--;
}

