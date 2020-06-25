#include"userSprite.h"

UserSprite::UserSprite(int x, int y, int w, int h, ACL_Image& img) :
	BaseSprite(x, y, w, h, img) {
	score = 0;
	strong = false;
	type = NONE;
	strongTime = 0;
}

UserSprite::~UserSprite() {

}

int UserSprite::getScore() {
	return score;
}

void UserSprite::addScore(int x) {
	if(score + x > 0) score += x;
	else score = 0;
}

	
void UserSprite::move(int key) {
	int speed;
	if (strong) speed = SPEED_USER *2;
	else speed = SPEED_USER;
	switch (key) {
	case VK_UP:
		if (y - speed >= 0) y -= speed;
		else y = 0;
		break;
	case VK_DOWN:
		if (y + speed + height <= WINHEIGHT_) y += speed;
		else y = WINHEIGHT_ - height;
		break;
	case VK_LEFT:
		if (x - speed >= 0) x -= speed;
		else x = 0;
		break;
	case VK_RIGHT:
		if (x + speed + height <= WINWIDTH_) x += speed;
		else x = WINWIDTH_ - width;
		break;
	}
}

OBJ UserSprite::crash(const BaseSprite& target) {
	OBJ res = target.type;
	bool check[4] = { false, false, false, false };
	if (x < target.x && x + width>target.x) check[0] = true; //左撞
	if (x > target.x && x < target.x + target.width) check[1] = true; //右撞
	if (y < target.y && y + height > target.y) check[2] = true; // 上撞
	if (y > target.y && y < target.y + target.height) check[3] = true; // 下撞
	if ((check[0] || check[1]) && (check[2] || check[3])) return res; //左撞右撞出现一个且上撞下撞出现一个
	res = NONE;
	return res;
}

bool UserSprite::isStrong() {
	return strong;
}

void UserSprite::getStrong() {
	strong = true;
	strongTime = STRONG_TIME;
}

void UserSprite::becomeWeak() {
	if (strongTime > 0)
		strongTime--;
	else
		strong = false;
}