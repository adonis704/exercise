#include"baseSprite.h"

BaseSprite::BaseSprite(int xpos, int ypos, int w, int h, ACL_Image& img) {
	x = xpos; y = ypos; width = w; height = h;
	image = img;
}

BaseSprite::~BaseSprite(){}

void BaseSprite::paint(){
	putImageScale(&image, x, y, width, height);
}

