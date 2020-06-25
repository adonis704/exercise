#include"potionSprite.h"

PotionSprite::PotionSprite(int x, int y, int w, int h, ACL_Image& img) :
	AutoSprite(x, y, w, h, img) {
	type = POTION;
}

PotionSprite::~PotionSprite(){}

void PotionSprite::move() {

}