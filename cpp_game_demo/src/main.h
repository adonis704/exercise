#ifndef main_h
#define main_h
#include<iostream>
#include<string>
#include"acllib.h"
#include"autoSprite.h"
#include"userSprite.h"
#include"avoidSprite.h"
#include"brickSprite.h"
#include"potionSprite.h"
#include"chaseSprite.h"
#define MAXNUMBER 15
#define MAXNUMBER_SAFE 20
#define DUCK_MAX_NUM 3
#define BRICK_MAX_NUM 6
#define POTION_MAX_NUM 1
#define CHASE_MAX_NUM 1
const int WINWIDTH = 1200;
const int WINHEIGHT = 800;
const int SPRITE_MOVE_GAP = 50;
const int SPRITE_CREATE_GAP = 1000;
const int SPRITE_WIDTH = 100;
const int SPRITE_HEIGHT = 100;
const int USER_WIDTH = 100;
const int USER_HEIGHT = 100;
const int USER_X = 600;
const int USER_Y = 400;
const int MOVE_GAP = 1;
const int JERRY_SCORE = 5;
const int DUCK_SCORE = 100;
const int DOG_SCORE = -2000;
const int BRICK_SCORE = -500;

int curNumber = 0;
int avoidNumber = 0;
int brickNumber = 0;
int potionNumber = 0;
int chaseNumber = 0;
int moveGap = MOVE_GAP;
int timer = 0;
double KDA = 0;
double scoreDouble = 0;
ACL_Image user, aut, avoid, brick, potion, dog;
AutoSprite *autoSprite[MAXNUMBER_SAFE] = { 0 };
UserSprite* userSprite = NULL;

void init();
void timerEvent(int id);
void keyEvent(int key, int event);
void createSprite();
void createSprite(int i);
void check();
void paint();

#endif
