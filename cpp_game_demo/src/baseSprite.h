#ifndef baseSprite_h
#define baseSprite_h
#include"acllib.h"
typedef enum {
	NONE,
	JERRY,
	BRICK,
	POTION,
	DUCK,
	DOG
} OBJ;

class BaseSprite {
public:
	int x, y, width, height;
	ACL_Image image;
	OBJ type;

	BaseSprite(int x, int y, int w, int h, ACL_Image& img);
	BaseSprite(){}
	~BaseSprite();

	void paint();
};
#endif