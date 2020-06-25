#include"main.h"

int Setup() {
	init();
	registerTimerEvent(timerEvent);
	registerKeyboardEvent(keyEvent);
	startTimer(0, SPRITE_MOVE_GAP);
	startTimer(1, SPRITE_CREATE_GAP);
	return 0;
}

void init() {
	initWindow("test", DEFAULT, DEFAULT, WINWIDTH, WINHEIGHT);
	//initConsole();
	loadImage("../pic/jerry.jpg", &aut);
	loadImage("../pic/tom.jpg", &user);
	loadImage("../pic/duck.jpg", &avoid);
	loadImage("../pic/brick.jpg", &brick);
	loadImage("../pic/potion.jpg", &potion);
	loadImage("../pic/dog.jpg", &dog);
	userSprite = new UserSprite(USER_X, USER_Y, USER_WIDTH, USER_HEIGHT, user);
}

void timerEvent(int id) {
	switch (id) {
	case 0:
		if (curNumber == 0) return;
		for (int i = 0; i < curNumber; i++) {
			if (autoSprite[i]->type == JERRY || autoSprite[i]->type == DUCK || autoSprite[i]->type == BRICK || autoSprite[i]->type == DOG) {
				autoSprite[i]->move();
				//std::cout << "move:" << i << std::endl;
				if (autoSprite[i]->type == BRICK && autoSprite[i]->life <= 0) {
					delete autoSprite[i];
					brickNumber--;
					createSprite(i);
			}
			}
		}		
		check();
		moveGap--;
		break;
	case 1:
		createSprite();
		userSprite->becomeWeak();
		timer++;
		break;		
	}
	paint();
}

void keyEvent(int key, int event) {
	if(!userSprite->isStrong())
		if (moveGap >= 0) return;
	if (event != KEY_DOWN) return;
	userSprite->move(key);
	moveGap = MOVE_GAP;
}

void createSprite() {
	if (curNumber >= MAXNUMBER) return;
	int x = rand() % (WINWIDTH - SPRITE_WIDTH);
	int y = rand() % (WINHEIGHT - SPRITE_HEIGHT);
	int temp = rand() % 100;
	if(temp < 60) autoSprite[curNumber++] = new AutoSprite(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, aut);
	else if (temp >= 60 && temp < 75 && avoidNumber < DUCK_MAX_NUM) {
		avoidNumber++;
		autoSprite[curNumber++] = new AvoidSprite(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, avoid, userSprite);		
	}
	else if (temp >= 75 && temp < 85 && brickNumber < BRICK_MAX_NUM) {
		brickNumber++;
		autoSprite[curNumber++] = new BrickSprite(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, brick);
	}
	else if (temp >= 85 && temp < 95 && potionNumber < POTION_MAX_NUM && !userSprite->isStrong()) {
		potionNumber++;
		autoSprite[curNumber++] = new PotionSprite(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, potion);
	}
	else if (temp >= 95 && chaseNumber < CHASE_MAX_NUM) {
		chaseNumber++;
		autoSprite[curNumber++] = new ChaseSprite(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, dog, userSprite);
	}
	else autoSprite[curNumber++] = new AutoSprite(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, aut);
	//std::cout << "createRaw:" << curNumber << std::endl;
}

void createSprite(int i) {
	int x = rand() % (WINWIDTH - SPRITE_WIDTH);
	int y = rand() % (WINHEIGHT - SPRITE_HEIGHT);
	int temp = rand() % 100;
	if (temp < 60) autoSprite[i] = new AutoSprite(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, aut);
	else if (temp >= 60 && temp < 75 && avoidNumber < DUCK_MAX_NUM) {
		avoidNumber++;
		autoSprite[i] = new AvoidSprite(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, avoid, userSprite);
		//std::cout << "create new duck:" << avoidNumber << std::endl;
	}
	else if (temp >= 75 && temp < 85 && brickNumber < BRICK_MAX_NUM) {
		brickNumber++;
		autoSprite[i] = new BrickSprite(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, brick);
	}
	else if (temp >= 85 && temp < 95 && potionNumber < POTION_MAX_NUM && !userSprite->isStrong()) {
		potionNumber++;
		autoSprite[i] = new PotionSprite(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, potion);
	}
	else if (temp >= 95 && chaseNumber < CHASE_MAX_NUM) {
		chaseNumber++;
		autoSprite[i] = new ChaseSprite(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, dog, userSprite);
	}
	else autoSprite[i] = new AutoSprite(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, aut);
	//std::cout << "createNew:" << i << std::endl;
}

void check() {
	for (int i = 0; i < curNumber; i++) {
		if (userSprite->crash(*autoSprite[i]) != NONE) {
			int score;
			switch (userSprite->crash(*autoSprite[i])) {
			case JERRY:
				score = JERRY_SCORE;
				break;
			case DUCK:
				score = DUCK_SCORE;
				avoidNumber--;
				break;
			case BRICK:
				if(!userSprite->isStrong()) score = BRICK_SCORE;
				else score = 0;
				brickNumber--;
				break;
			case POTION:
				score = 0;
				userSprite->getStrong();
				potionNumber--;
				break;
			case DOG:
				score = DOG_SCORE;
				chaseNumber--;
				break;
			default:
				break;
			}
			userSprite->addScore(score);
			delete autoSprite[i];
			createSprite(i);
		}
	}
}

void paint() {
	beginPaint();
	clearDevice();

	userSprite->paint();
	scoreDouble = userSprite->getScore();
	if(timer != 0) 	KDA =  scoreDouble/ timer;
	for (int i = 0; i < curNumber; i++)
		autoSprite[i]->paint();
	setTextSize(20);
	paintText(10, 10, "�÷�:");
	paintText(60, 10, std::to_string(userSprite->getScore()).c_str());
	paintText(200, 10, "����ʣ��ʱ��:");
	paintText(340, 10, std::to_string(userSprite->strongTime).c_str());
	paintText(450, 10, "��Ϸʱ��:");
	paintText(550, 10, std::to_string(timer).c_str());
	paintText(650, 10, "����÷�:");
	paintText(750, 10, std::to_string(KDA).c_str());
	
	endPaint();
}