#include"avoidSprite.h"
#include<math.h>

AvoidSprite::AvoidSprite(int x, int y, int w, int h, ACL_Image& img, UserSprite* t):
	 AutoSprite(x, y, w, h, img) {
	type = DUCK;
	target = t;
	gap = 0;
}

AvoidSprite::~AvoidSprite() {

}

void AvoidSprite::move() {
	if(gap <= 0)
		if (danger()) {
			dx *= -1;
			dy *= -1;
			gap = GAP_VALUE;
		}
	if(gap >0) gap--;

	x += SPEED_AVOID * dx;
	y += SPEED_AVOID * dy;
	if (x < 0) x = WINWIDTH__ - width;
	if (x + width > WINWIDTH__) x = 0;
	if (y < 0) y = WINHEIGHT__ - height;
	if (y + height > WINHEIGHT__) y = 0;
}

bool AvoidSprite::danger() {
	int avoidX, avoidY, userX, userY;
	double dist;

	avoidX = x + width / 2;
	avoidY = y + height / 2;
	userX = target->x + target->width / 2;
	userY = target->y + target->height / 2;
	dist = sqrt((avoidX - userX)*(avoidX - userX) + (avoidY - userY)*(avoidY - userY));

	if (dist < DIST_SAFE) return true;
	return false;	
}