#include"autoSprite.h"

AutoSprite::AutoSprite(int x, int y, int w, int h, ACL_Image& img)
	:BaseSprite(x, y, w, h, img) {
	type = JERRY;
	int temp = rand() % 100;
	if (temp < 25) {
		dx = 1;
		dy = 1;
	}
	else if (temp >= 25 && temp < 50) {
		dx = 1;
		dy = -1;
	}
	else if (temp >= 50 && temp < 75) {
		dx = -1;
		dy = 1;
	}
	else {
		dx = -1;
		dy = -1;
	}
}

AutoSprite::~AutoSprite() {

}

void AutoSprite::move() {
	x += SPEED_AUTO * dx;
	y += SPEED_AUTO * dy;
	if (x < 0) x = WINWIDTH__ - width;
	if (x + width > WINWIDTH__) x = 0;
	if (y < 0) y = WINHEIGHT__ - height;
	if (y + height > WINHEIGHT__) y = 0;
}