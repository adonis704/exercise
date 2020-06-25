#include"chaseSprite.h"
#include<math.h>

ChaseSprite::ChaseSprite(int x, int y, int w, int h, ACL_Image& img, UserSprite* t) :
	AutoSprite(x, y, w, h, img) {
	type = DOG;
	target = t;
	gap = 0;
}

ChaseSprite::~ChaseSprite() {

}

void ChaseSprite::move() {
	int xp = x + width / 2;
	int yp = y + height / 2;
	int txp = target->x + target->width / 2;
	int typ = target->y + target->height / 2;
	double tan;
	if (xp - txp == 0) tan = TAN_MAX;
	else tan = abs(yp - typ) / abs(xp - txp);
	if (tan > TAN_MAX) tan = TAN_MAX;
	bool bigerX = xp > txp;
	bool bigerY = yp > typ;

	if (bigerX && bigerY) {
		x -= SPEED_CHASE;
		y -= SPEED_CHASE * tan;
		return;
	}
	if (!bigerX && bigerY) {
		x += SPEED_CHASE;
		y -= SPEED_CHASE * tan;
		return;
	}
	if (bigerX && !bigerY) {
		x -= SPEED_CHASE;
		y += SPEED_CHASE * tan;
		return;
	}
	if (!bigerX && !bigerY) {
		x += SPEED_CHASE;
		y += SPEED_CHASE * tan;
		return;
	}
}