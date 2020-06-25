#ifndef userSprite_h
#define userSprite_h
#include"acllib.h"
#include"baseSprite.h"
const int SPEED_USER = 10;
const int STRONG_TIME = 30;
const int WINWIDTH_ = 1200;
const int WINHEIGHT_ = 800;

class UserSprite:public BaseSprite {
private:
	int score;
	bool strong;

public:
	int strongTime;
	UserSprite(int x, int y, int w, int h, ACL_Image& img);
	UserSprite(){}
	~UserSprite();
	void move(int key);
	int getScore();
	void addScore(int x);
	OBJ crash(const BaseSprite& target);
	bool isStrong();
	void getStrong();
	void becomeWeak();

};
#endif